/*
 * A048853  Number of primes (different from n) that can be produced by
 *          altering one digit of decimal expansion of n (without changing the
 *          number of digits).
 * https://oeis.org/A048853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy, nice
 * Formula:   a(n) = number of primes (different from n) obtainable by *
 *            altering exactly one decimal digit of n without changing the
 *            number of * digits. This implementation builds each possible
 *            candidate number, * tests it with is_prime_big, and counts the
 *            primes
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_len.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Determine decimal length and store digits. 20 digits covers
     * the maximum 64‑bit unsigned integer. */
    int d = digit_len(n);
    uint8_t digs[20];
    idx_t tmp = n;
    for (int i = d - 1; i >= 0; --i) {
        digs[i] = tmp % 10;
        tmp /= 10;
    }

    seq_t count = 0;

    for (int pos = 0; pos < d; ++pos) {
        uint8_t orig = digs[pos];
        for (int nd = 0; nd <= 9; ++nd) {
            if (nd == orig) continue;                 /* digit unchanged */
            if (pos == 0 && nd == 0) continue;        /* avoid leading zero */

            /* Build the candidate number digit by digit. */
            seq_t cand = 0;
            for (int i = 0; i < d; ++i) {
                uint8_t dig = (i == pos) ? nd : digs[i];
                cand = cand * 10 + dig;
            }

            if (is_prime_big(cand))
                ++count;
        }
    }
    return count;
}
