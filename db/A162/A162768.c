/*
 * A162768  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of reduced words of length n in the Coxeter *
 *            group on 13 generators S_i with relations (S_i)^2 = (S_i S_j)^3
 *            = I. * Generating function: (t^3 + 2*t^2 + 2*t + 1)/(66*t^3 -
 *            11*t^2 - 11*t + 1). * The corresponding linear recurrence is *
 *            a(n) = 11*a(n-1) + 11*a(n-2) - 66*a(n-3) for n >...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four values explicitly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)13;
    if (n == 2)
        return (seq_t)156;
    if (n == 3)
        return (seq_t)1794;

    /* Last three computed terms:
     *  a_nm3 = a(n-3), a_nm2 = a(n-2), a_nm1 = a(n-1)   */
    seq_t a_nm3 = (seq_t)13;   /* a(1) */
    seq_t a_nm2 = (seq_t)156;  /* a(2) */
    seq_t a_nm1 = (seq_t)1794; /* a(3) */

    seq_t a_curr;
    for (idx_t i = 4; i <= n; ++i)
    {
        /* a(i) = 11*a(i-1) + 11*a(i-2) - 66*a(i-3) */
        a_curr = 11 * a_nm1 + 11 * a_nm2 - 66 * a_nm3;

        /* Shift the window for the next iteration. */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_curr;
    }

    return a_nm1;
}
