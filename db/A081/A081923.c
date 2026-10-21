/*
 * A081923  Expansion of e.g.f.: exp(2x)/(1-x)^2.
 * https://oeis.org/A081923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+3)*a(n-1) - 2*(n-1)*a(n-2) with initial values a(0) =
 *            1, a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first two terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* iterate using the recurrence */
    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)4;   /* a(1) */
    seq_t a_n = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* a(n) = (n+3)*a(n-1) - 2*(n-1)*a(n-2) */
        seq_t term1 = a_nm1 * (seq_t)(i + 3);
        seq_t term2 = a_nm2 * (seq_t)((i - 1) * 2);
        a_n = term1 - term2;

        /* shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
