/*
 * A161225  a(n) = number of distinct integers that can be constructed by
 *          removing one or more 0's from the binary representation of n, and
 *          concatenating while leaving the remaining digits in their same
 *          order.
 * https://oeis.org/A161225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look, hear
 * Formula:   a(n) = (∏ over zero‑runs (run_length+1)) – 1 where the product
 *            is over all maximal consecutive runs of zeroes in the binary
 *            representation of n (with a(n)=0 if n has no zeroes)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n=0, but safe to return 0 */

    seq_t prod = 1;                /* product of (run_length+1) */
    idx_t zeros = 0;               /* length of current zero run */

    /* Find index of the most significant set bit (msb) */
    idx_t msb = 0;
    {
        idx_t tmp = n;
        while (tmp > 1) {
            tmp >>= 1;
            msb++;
        }
    }

    /* Scan bits from msb down to 0 */
    for (idx_t i = msb; ; i--) {
        bool bit = (n >> i) & 1;

        if (bit) {
            if (zeros > 0) {
                prod *= (seq_t)(zeros + 1);
                zeros = 0;
            }
        } else {
            zeros++;
        }

        if (i == 0) break;          /* finished least significant bit */
    }

    /* If the number ended with a zero run, account for it */
    if (zeros > 0)
        prod *= (seq_t)(zeros + 1);

    return prod - 1;                 /* exclude the case where no zero is removed */
}
