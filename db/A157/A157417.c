/*
 * A157417  Primes of the form floor((4*n^2-8*n-9)/3).
 * https://oeis.org/A157417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 75, u32 n <= 126, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes of the form floor((4*n^2-8*n-9)/3) and return the
 * nth such prime (1‑based). */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Sanity: n must be >= 1 */
    if (n == 0) return 0;

    idx_t found = 0;          /* how many primes found so far */
    for (idx_t k = 1; ; ++k) {
        /* compute numerator as signed 64‑bit to avoid unsigned wrap‑around */
        int64_t num = (int64_t)4 * (int64_t)k * (int64_t)k
                      - (int64_t)8 * (int64_t)k - 9;
        if (num >= 0) {
            seq_t val = (seq_t)(num / 3);   /* floor division */
            if (val > 1 && is_prime_big(val)) {
                ++found;
                if (found == n)
                    return val;
            }
        }
    }
}
