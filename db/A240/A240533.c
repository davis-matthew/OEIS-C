/*
 * A240533  a(n) = numerators of n!/10^n.
 * https://oeis.org/A240533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of n! / 10^n = n! / (2^min(v_2(n!),n) *
 *            5^min(v_5(n!),n))
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 18, u64 n <= 29, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 1;
    idx_t rem2 = 0, rem5 = 0;          /* how many 2's and 5's have been removed */

    for (idx_t i = 1; i <= n; ++i) {
        idx_t j = i;

        /* remove as many 2's as possible, but not more than n total */
        while (rem2 < n && (j & 1ULL) == 0ULL) {
            j >>= 1;
            ++rem2;
        }

        /* remove as many 5's as possible, but not more than n total */
        while (rem5 < n && j % 5ULL == 0ULL) {
            j /= 5;
            ++rem5;
        }

        res *= (seq_t)j;
    }

    return res;
}
