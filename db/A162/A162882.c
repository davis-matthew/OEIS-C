/*
 * A162882  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 44, a(2) = 1892, a(3) = 80410; a(n) = 42*a(n-1)
 *            + 42*a(n-2) - 903*a(n-3) for n >= 4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)44;
    if (n == 2u) return (seq_t)1892;
    if (n == 3u) return (seq_t)80410;

    /* sliding window for recurrence */
    seq_t a_nm3 = (seq_t)44;   /* a(n-3) when i==4 */
    seq_t a_nm2 = (seq_t)1892; /* a(n-2) */
    seq_t a_nm1 = (seq_t)80410;/* a(n-1) */

    seq_t current;
    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t term1 = (seq_t)42 * a_nm1;
        seq_t term2 = (seq_t)42 * a_nm2;
        seq_t term3 = (seq_t)903 * a_nm3;
        current = term1 + term2 - term3;

        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = current;
    }

    return a_nm1;
}
