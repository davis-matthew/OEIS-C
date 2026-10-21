/*
 * A064399  Numbers k whose sum of digits exceeds the sum of the digits of
 *          k^2.
 * https://oeis.org/A064399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the nth integer k for which digit_sum(k) > digit_sum(k^2) */
#include <stdint.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Return the nth term of A064399. */
    if (n == 0) {
        return 0;  /* Undefined for n = 0, but return 0 to avoid UB. */
    }

    idx_t count = 0;          /* Number of qualifying numbers found. */
    seq_t k = 0;              /* Candidate number. */

    while (count < n) {
        k++;
        seq_t k_sq = k * k;
        if (digit_sum(k) > digit_sum(k_sq)) {
            count++;
        }
    }

    return k;
}
