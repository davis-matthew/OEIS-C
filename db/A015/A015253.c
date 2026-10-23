/*
 * A015253  Gaussian binomial coefficient [ n,2 ] for q = -4.
 * https://oeis.org/A015253
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/75)*((-4)^n - 1)*(1 - (-4)^(n-1)) Recurrence: a(n) =
 *            13*a(n-1) + 52*a(n-2) - 64*a(n-3), n>=5 Initial values: a(2)=1,
 *            a(3)=13, a(4)=221
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)13;
    if (n == 4)
        return (seq_t)221;

    seq_t a_nm3 = (seq_t)1;    /* a(2) */
    seq_t a_nm2 = (seq_t)13;   /* a(3) */
    seq_t a_nm1 = (seq_t)221;  /* a(4) */
    seq_t a_val;

    for (idx_t i = 5; i <= n; ++i)
    {
        a_val = (seq_t)13 * a_nm1
                + (seq_t)52 * a_nm2
                - (seq_t)64 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_val;
    }
    return a_val;
}
