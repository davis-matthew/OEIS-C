/*
 * A192940  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x)=(x+2)(x+4)...(x+2n).
 * https://oeis.org/A192940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = (4*n-1)*a(n-1) - (4*n*n-6*n+1)*a(n-2),
   with a(0)=0, a(1)=1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t a_nm2 = (seq_t)0; /* a(0) */
    seq_t a_nm1 = (seq_t)1; /* a(1) */
    seq_t a_n   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* term1 = (4*i-1) * a(n-1)  */
        seq_t mul1 = a_nm1 * (seq_t)(4 * i - 1);

        /* term2 = (4*i*i - 6*i + 1) * a(n-2) */
        idx_t ii   = i * i;
        seq_t coeff2 = (seq_t)(4 * ii - 6 * i + 1);
        seq_t mul2 = a_nm2 * coeff2;

        a_n = mul1 - mul2; /* a(n) = term1 - term2 */

        /* shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
