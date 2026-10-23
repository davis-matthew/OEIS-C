/*
 * A038155  a(n) = (n!/2) * Sum_{k=0..n-2} 1/k!.
 * https://oeis.org/A038155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n!/2) * sum_{k=0}^{n-2} 1/k!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used: a(n) = (n+1)*a(n-1) - (n-1)*a(n-2) + (n-1), a(0)=a(1)=0 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0 || n == 1)
        return 0;

    seq_t a_nm2 = 0;        /* a(n-2) */
    seq_t a_nm1 = 0;        /* a(n-1) */
    seq_t a_n   = 0;        /* a(n)  */

    for (idx_t i = 2; i <= n; ++i)
    {
        a_n = ((seq_t)(i + 1)) * a_nm1
            - ((seq_t)(i - 1)) * a_nm2
            + ((seq_t)(i - 1));

        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
