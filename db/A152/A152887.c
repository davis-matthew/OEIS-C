/*
 * A152887  Number of descents beginning with an even number and ending with
 *          an odd number in all permutations of {1,2,...,n}.
 * https://oeis.org/A152887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)!*(2*n*(n+1)+(2*n+1)*(-1)^n-1)/16 Recurrence:
 *            a(1)=0, a(2)=1, a(3)=2 a(n) = 2*a(n-1) + (n^2-2)*a(n-2) -
 *            n*(n-3)*a(n-3) for n >= 4
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three known terms directly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;

    /* Maintain the last three values of the sequence */
    seq_t a_nm3 = (seq_t)0; /* a(1) */
    seq_t a_nm2 = (seq_t)1; /* a(2) */
    seq_t a_nm1 = (seq_t)2; /* a(3) */
    seq_t a_n   = 0;        /* placeholder for a(n) */

    /* Iterate from n = 4 up to the desired index */
    for (idx_t i = 4; i <= n; ++i)
    {
        /* Compute (i^2 - 2) and n*(n-3) as seq_t */
        seq_t sq = (seq_t)(i * i);                 /* i^2 fits in 64-bit */
        seq_t coeff1 = sq - 2;                     /* (i^2 - 2) */
        seq_t coeff2 = (seq_t)i * (seq_t)(i - 3);   /* i*(i-3) */

        /* a(n) = 2*a(n-1) + (i^2-2)*a(n-2) - i*(i-3)*a(n-3) */
        a_n = 2 * a_nm1 + coeff1 * a_nm2 - coeff2 * a_nm3;

        /* Shift the window for the next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
