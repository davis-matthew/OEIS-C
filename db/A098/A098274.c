/*
 * A098274  Sum_{k = 0..n} C(n, k)^2*C(n+k, n)*C(n+2*k, n).
 * https://oeis.org/A098274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = Sum_{k = 0..n} C(n,k)^2 * C(n+k,n) * C(n+2*k,n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t c1 = binom(n, k);                   /* C(n,k)          */
        seq_t term = c1 * c1;                     /* C(n,k)^2        */
        term *= binom(n + k, n);                  /* * C(n+k,n)      */
        term *= binom(n + 2 * k, n);              /* * C(n+2k,n)     */
        sum += term;
    }
    return sum;
}
