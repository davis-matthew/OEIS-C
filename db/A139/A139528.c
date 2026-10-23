/*
 * A139528  Numbers k such that 24*k + 11 is prime.
 * https://oeis.org/A139528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 803, u32 n <= 804, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th integer k such that 24*k+11 is prime. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for n < 1; spec says offset is 1 */

    idx_t count = 0;
    idx_t k = 0;

    while (1) {
        /* compute 24*k + 11 in a wider unsigned type to avoid overflow on k */
        uint64_t cand_u64 = 24ULL * (uint64_t)k + 11ULL;
        seq_t cand = (seq_t)cand_u64;

        if (is_prime_big(cand)) {
            count++;
            if (count == n) {
                return (seq_t)k;   /* kth value that satisfies the condition */
            }
        }
        ++k;
    }
}
