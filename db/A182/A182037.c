/*
 * A182037  Expansion of 1 - (1 - 2*x - x^2)^(1/2).
 * https://oeis.org/A182037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, a(2) = 2 a(n) = (2*n-3) * a(n-1) +
 *            (n-1)*(n-3) * a(n-2) for n >= 3 (Equivalent to the linear
 *            recurrence found in the OEIS entry)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

// Assuming seq_t and idx_t are defined by the build system.
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t a_nm2 = (seq_t)1;   /* a(1) */
    seq_t a_nm1 = (seq_t)2;   /* a(2) */
    seq_t a_i   = a_nm1;      /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        /* compute coefficients (converted to seq_t) */
        seq_t coeff1 = (seq_t)( (seq_t)(2 * i) - (seq_t)3 );               /* 2*i-3 */
        seq_t coeff2 = (seq_t)( ( (seq_t)(i - 1) ) * ( (seq_t)(i - 3) ) ); /* (i-1)*(i-3) */

        /* a(i) = coeff1*a(i-1) + coeff2*a(i-2) */
        a_i = coeff1 * a_nm1 + coeff2 * a_nm2;

        /* shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_i;
    }

    return a_i;
}
