/*
 * A084543  a(2,n) as defined in A003148.
 * https://oeis.org/A084543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -3*a(n-1) + 2*(2*n+3)*(n-1)*a(n-2) with a(0) = 3, a(1) =
 *            -9, as defined for sequence A084543
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial two terms explicitly. */
    if (n == 0)
        return 3;
    if (n == 1)
        return -9;

    /* a(n-2) and a(n-1) for the iterative recurrence. */
    seq_t a_nm2 = 3;   /* a(0) */
    seq_t a_nm1 = -9;  /* a(1) */
    seq_t a_n;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* Compute the polynomial coefficient:
           2 * (2*i + 3) * (i - 1). */
        seq_t coeff = 2;
        coeff *= ((seq_t)2 * i + 3);
        coeff *= (i - 1);

        /* Recurrence: a(n) = -3*a(n-1) + coeff * a(n-2). */
        a_n = -(3 * a_nm1) + coeff * a_nm2;

        /* Shift the window for next iteration. */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
