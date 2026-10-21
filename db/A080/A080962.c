/*
 * A080962  5th binomial transform of the periodic sequence (1,6,1,1,6,1...).
 * https://oeis.org/A080962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = 6·a(n‑1) + 5·4^(n‑1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base case */
    if (n == 0)
        return (seq_t)1;

    seq_t a_n = (seq_t)1;   /* a(0) */
    seq_t pow4 = (seq_t)1;  /* 4^(n‑1) with n starting at 1 */

    for (idx_t i = 1; i <= n; ++i)
    {
        a_n = a_n * (seq_t)6 + pow4 * (seq_t)5;
        pow4 *= (seq_t)4;
    }

    return a_n;
}
