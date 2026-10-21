/*
 * A073141  Product of the largest and smallest number having in binary
 *          representation the same number of 0's and 1's as n.
 * https://oeis.org/A073141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest(n) * largest(n) * where smallest(n) is the
 *            minimal integer with the same number of * 1-bits as n and the
 *            same bit-length, and largest(n) is the maximal * such integer.
 *            The product is computed in the type seq_t
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    // Compute bit-length (at least 1) and popcount of n
    idx_t bits = 0;
    idx_t t = n;
    if (t == 0) {
        bits = 1;                     /* representation of 0 is "0" */
    } else {
        while (t) {
            bits++;
            t >>= 1;
        }
    }

    idx_t ones = 0;
    t = n;
    while (t) {
        ones++;
        t &= t - 1;                   /* clear lowest set bit */
    }

    /* n == 0 results in 0, consistent with the OEIS definition */
    if (n == 0) {
        return (seq_t)0;
    }

    idx_t L = bits;                   /* bit-length */
    idx_t k = ones;                   /* number of 1-bits */

    /* Compute the smallest number:
     *  - leading bit (most significant) is 1
     *  - remaining (k-1) 1-bits are placed at the least significant positions
     */
    seq_t small = 0;
    small |= ((seq_t)1ULL) << (L - 1);
    for (idx_t i = 0; i < k - 1; ++i) {
        small |= ((seq_t)1ULL) << i;
    }

    /* Compute the largest number:
     *  - leading bit is 1
     *  - the remaining (k-1) 1-bits are placed as high as possible
     */
    seq_t large = 0;
    large |= ((seq_t)1ULL) << (L - 1);
    for (idx_t i = 1; i < k; ++i) {
        large |= ((seq_t)1ULL) << (L - 1 - i);
    }

    return small * large;
}
