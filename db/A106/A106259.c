/*
 * A106259  Expansion of 1/sqrt(1-12x-12x^2).
 * https://oeis.org/A106259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   n*a(n) = 6*(2*n-1)*a(n-1) + 12*(n-1)*a(n-2); with a(0)=1, a(1)=6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)6;

    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)6;   /* a(1) */
    seq_t a_n   = a_nm1;      /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        /* Compute a(i) using the recurrence */
        seq_t term1 = (seq_t)(12 * i - 6) * a_nm1;
        seq_t term2 = (seq_t)12 * (i - 1) * a_nm2;
        a_n = (term1 + term2) / i;
        /* shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
