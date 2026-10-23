/*
 * A167193  a(n) = (1/3)*(1 - (-2)^n + 3*(-1)^n ) = (-1)^(n+1)*A167030(n).
 * https://oeis.org/A167193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (-1)^(n+1) * A167030(n)
 * Recurrence: a(0)=1, a(1)=0, a(2)=0
 *             a(n) = -a(n-1) + 2*a(n-2) - 2*(-1)^n   (n ≥ 3)
 */
seq_t a(idx_t n)
{
    /* Handle the first three base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;

    /* a_nm2 = a(n-2), a_nm1 = a(n-1) */
    seq_t a_nm2 = (seq_t)1;  /* a(0) */
    seq_t a_nm1 = (seq_t)0;  /* a(1) */
    seq_t a_n   = 0;         /* placeholder for current term */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t sign = ( (i & 1) ? (seq_t)-1 : (seq_t)1 );  /* (-1)^i */
        a_n = -a_nm1 + ( (seq_t)2 * a_nm2 ) - ( (seq_t)2 * sign );

        /* shift for next iteration: a(n-2) <- a(n-1), a(n-1) <- a(n) */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
