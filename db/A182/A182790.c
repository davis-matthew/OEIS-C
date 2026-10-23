/*
 * A182790  Number of n-colorings of the 5 X 5 X 5 triangular grid.
 * https://oeis.org/A182790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n^9 -21*n^8 +198*n^7 -1102*n^6 +3999*n^5
 *            -9840*n^4 +16475*n^3 -18177*n^2 +12056*n -3686)*(n-2)^4 The
 *            implementation evaluates the polynomial directly using 64‑bit
 *            intermediate powers (cast to seq_t). It returns 0 for n < 3 as
 *            per the offset definition
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 21, u128 n <= 372,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t sn   = (seq_t)n;
    seq_t sn1  = sn - 1;
    seq_t sn2  = sn - 2;

    /* Compute powers of n up to n^9 */
    seq_t n2  = sn * sn;
    seq_t n3  = n2 * sn;
    seq_t n4  = n3 * sn;
    seq_t n5  = n4 * sn;
    seq_t n6  = n5 * sn;
    seq_t n7  = n6 * sn;
    seq_t n8  = n7 * sn;
    seq_t n9  = n8 * sn;

    /* Polynomial evaluation */
    seq_t poly =
        n9
        - (seq_t)21 * n8
        + (seq_t)198 * n7
        - (seq_t)1102 * n6
        + (seq_t)3999 * n5
        - (seq_t)9840 * n4
        + (seq_t)16475 * n3
        - (seq_t)18177 * n2
        + (seq_t)12056 * sn
        - (seq_t)3686;

    /* (n-2)^4 */
    seq_t sn2_sq = sn2 * sn2;
    seq_t pow4   = sn2_sq * sn2_sq;

    /* Final product */
    return sn * sn1 * poly * pow4;
}
