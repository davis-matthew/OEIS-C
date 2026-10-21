/*
 * A051789  a(n) = C(n)*(C(n)-1)/2, where C(n) are the Catalan numbers
 *          (A000108).
 * https://oeis.org/A051789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n)*(C(n)-1)/2, where C(n) = binom(2*n,n)/(n+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Compute Catalan number C(n) = binom(2n,n)/(n+1) */
    seq_t cat = binom(2 * n, n);
    /* Avoid accidental shift-of-idx overflow   */
    cat /= (seq_t)(n + 1);

    /* For n = 0 or 1, cat == 1, so a(n) == 0 automatically */
    return (cat * (cat - 1)) / 2;
}
