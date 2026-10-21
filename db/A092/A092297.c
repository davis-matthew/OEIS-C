/*
 * A092297  Number of ways of 3-coloring an annulus consisting of n zones
 *          joined like a pearl necklace.
 * https://oeis.org/A092297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A092297
 *  Formula: a(n) = 2^n + 2*(-1)^n   (offset 1, a(1)=0)
 *  The implementation uses the provided pow2 helper to compute 2^n.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Undefined for n <= 0 – return 0 as a safe default */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Compute 2^n */
    seq_t res = pow2(n);

    /* Add or subtract 2 depending on the parity of n */
    if (n & 1) {
        res -= (seq_t)2;   /* (-1)^n = -1 for odd n */
    } else {
        res += (seq_t)2;   /* (-1)^n = +1 for even n */
    }

    return res;
}
