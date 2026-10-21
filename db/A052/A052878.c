/*
 * A052878  E.g.f.: log((1-x)/(1-3*x+x^2)).
 * https://oeis.org/A052878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n-1)! * (phi^(2*n)+phi^(-2*n)-1) where phi =
 *            (1+sqrt(5))/2 Offset: 0 (a(0) = 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;

    /* compute (n-1)! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i < n; ++i)
        fact *= (seq_t)i;

    /* compute Lucas number L_{2n} */
    const idx_t m = 2 * n;          /* index of Lucas number needed */
    seq_t L_prevprev = (seq_t)2;    /* L0 */
    seq_t L_prev     = (seq_t)1;    /* L1 */
    seq_t L_curr     = 0;

    /* For n >= 1, we have 2*n >= 2, so we can start from L2 */
    for (idx_t k = 2; k <= m; ++k) {
        L_curr = L_prevprev + L_prev;
        L_prevprev = L_prev;
        L_prev = L_curr;
    }

    seq_t term = L_curr - (seq_t)1;
    return fact * term;
}
