/*
 * A048678  Binary expansion of nonnegative integers expanded to
 *          "Zeckendorffian format" with rewrite rules 0->0, 1->01.
 * https://oeis.org/A048678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; a(2n)=2*a(n); a(2n+1)=4*a(n)+1 (binary rewrite 0→0,
 *            1→01)
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* Find number of bits in n (msb position + 1) */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp != 0) {
        msb++;
        tmp >>= 1;
    }

    seq_t result = (seq_t)0;
    /* Process bits from the most significant to the least significant */
    for (idx_t i = msb; i > 0; ) {
        --i;
        if ((n >> i) & (idx_t)1) {
            /* Bit is 1: a(2k+1) = 4*a(k) + 1 */
            result = result * (seq_t)4 + (seq_t)1;
        } else {
            /* Bit is 0: a(2k) = 2*a(k) */
            result = result * (seq_t)2;
        }
    }
    return result;
}
