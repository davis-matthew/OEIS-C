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
 * Validated: standard, pass, 121 terms through n = 9235 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Determine decimal length and store digits. 20 digits covers
     * the maximum 64‑bit unsigned integer. */
    int d = num_digits(n);
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
