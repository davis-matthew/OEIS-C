/*
 * A103371  Number triangle T(n,k) = C(n,n-k)*C(n+1,n-k).
 * https://oeis.org/A103371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = C(r, k) * C(r+1, k+1) where r = row_of(n) (the unique r
 *            with r(r+1)/2 ≤ n < (r+1)(r+2)/2) and k = n - r(r+1)/2
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 179, u64 n <= 644,
 *            u128 n <= 2309, big any n
 * Helpers:   binom, row_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* helpers provided by the build environment */
#include "../utils/binom.h"
#include "../utils/row_of.h"

seq_t a(idx_t n)
{
    idx_t r = row_of(n);                           /* row index */
    idx_t k = n - r * (r + 1) / 2;                 /* column index */
    seq_t b1 = binom(r, k);                        /* C(r, k)    */
    seq_t b2 = binom(r + 1, k + 1);                /* C(r+1, k+1)*/
    return b1 * b2;                                /* T(r, k)    */
}
