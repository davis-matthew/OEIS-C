/*
 * A061338  Increase in maximal number of comparisons for sorting n elements
 *          by list merging.
 * https://oeis.org/A061338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; for n>0 a(n) = (n & -n) + popcount(n) - 1 (where n&-n is
 *            the lowest power of two dividing n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* lowest power of 2 dividing n */
    seq_t low = (seq_t)(n & -n);

    /* popcount of n */
    unsigned long pop = 0;
    idx_t tmp = n;
    while (tmp) {
        tmp &= tmp - 1;
        ++pop;
    }

    return low + (seq_t)(pop - 1);
}
