/*
 * A143165  Expansion of the exponential generating function
 *          arcsin(2*x)/(2*(1-2*x)^(3/2)).
 * https://oeis.org/A143165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 2*(n+1)*(3+2*n)^2*a(n) - (4*n^2+8*n+1)*a(n+1)
 *              - (2*(n+4))*a(n+2) + a(n+3) = 0
 * with initial values a(0)=0, a(1)=1, a(2)=6, a(3)=49
 * This implements the OEIS sequence A143165. */

seq_t a(idx_t n)
{
    /* handle the first four explicitly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)49;

    /* a0 = a(i), a1 = a(i+1), a2 = a(i+2) */
    seq_t a0 = (seq_t)0; /* a(0) */
    seq_t a1 = (seq_t)1; /* a(1) */
    seq_t a2 = (seq_t)6; /* a(2) */

    for (idx_t i = 0; i <= n - 3; ++i)
    {
        seq_t ii  = (seq_t)i;
        /* coefficients in seq_t */
        seq_t coeff1 = (seq_t)(4) * ii * ii + (seq_t)(8) * ii + (seq_t)1;
        seq_t coeff2 = (seq_t)(2) * (seq_t)(i + 4);
        seq_t coeff3 = (seq_t)(2) * (seq_t)(i + 1);

        seq_t t32 = (seq_t)(3 + 2 * i);     /* 3 + 2*i */
        seq_t term3 = coeff3 * t32 * t32 * a0;

        seq_t next = coeff1 * a1 + coeff2 * a2;
        next -= term3;

        /* shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    /* After the loop, a2 holds a(n) */
    return a2;
}
