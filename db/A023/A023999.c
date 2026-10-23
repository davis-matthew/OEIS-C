/*
 * A023999  Absolute value of determinant of n X n matrix whose entries are
 *          the integers from 1 to n^2 spiraling inward, starting in a corner.
 * https://oeis.org/A023999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3n-1) * (2n-3)!/(n-2)! for n >= 2, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 1;          /* a(1) = 1, a(0) is unused */
    /* multiply the product (n-1)*(n)*...*(2n-3) */
    seq_t prod = 1;
    idx_t start = n - 1;
    idx_t end   = 2 * n - 3;
    for (idx_t k = start; k <= end; ++k) {
        prod *= (seq_t)k;
    }
    /* multiply by (3n-1) */
    prod *= (3 * n - 1);
    return prod;
}
