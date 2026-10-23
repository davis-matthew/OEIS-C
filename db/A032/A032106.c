/*
 * A032106  Number of reversible strings with n black beads and n-1 white
 *          beads. String is not palindromic.
 * https://oeis.org/A032106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = (C(4n,2n) - C(2n,n)) / 4 Formula: a(2n+1) =
 *            (C(4n+1,2n+1) - C(2n,n)) / 2 a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* a(1) is a special case */
    if (n == 1) return (seq_t)1;

    /* n = 2*m        -> even case
       n = 2*m + 1    -> odd case   */
    idx_t m = n >> 1;          /* m = floor(n/2) */

    /* Common term C(2m, m) */
    seq_t c_common = binom(2 * m, m);

    if ((n & 1) == 0) {        /* even n = 2*m */
        seq_t c_big = binom(4 * m, 2 * m);   /* C(4m, 2m) */
        return (c_big - c_common) / (seq_t)4;
    } else {                   /* odd n = 2*m + 1 */
        seq_t c_big = binom(4 * m + 1, 2 * m + 1);  /* C(4m+1, 2m+1) */
        return (c_big - c_common) / (seq_t)2;
    }
}
