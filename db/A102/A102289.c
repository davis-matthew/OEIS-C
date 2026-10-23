/*
 * A102289  Total number of odd lists in all sets of lists, cf. A000262.
 * https://oeis.org/A102289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*a(n-1) + n^2*a(n-2) - (n-2)^2*n*a(n-3), a(0) = 0, a(1)
 *            = 1, a(2) = 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;

    seq_t a_nm3 = 0;   /* a(0) */
    seq_t a_nm2 = 1;   /* a(1) */
    seq_t a_nm1 = 2;   /* a(2) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t term1 = (seq_t)i * a_nm1;
        seq_t term2 = (seq_t)i * (seq_t)i * a_nm2;

        seq_t d = (seq_t)(i - 2);            /* (i-2) as seq_t */
        seq_t term3 = d * d * (seq_t)i * a_nm3;

        seq_t val = term1 + term2;
        val -= term3;                         /* safe: recurrence guarantees non‑negative */

        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = val;
    }
    return a_nm1;
}
