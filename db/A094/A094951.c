/*
 * A094951  a(n) = A081038(n) + A077616(n).
 * https://oeis.org/A094951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 6 a(n) = 3^(n-2) * (9 + 7*n + 2*n^2) for n >= 2 (n is
 *            1‑based index)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* below offset, value irrelevant */
        return 0;
    if (n == 1)          /* first term */
        return (seq_t)6;

    /* Compute 3^(n-2) */
    seq_t pow = 1;
    for (idx_t i = 0; i < n - 2; ++i)
        pow *= (seq_t)3;

    seq_t n_seq = (seq_t)n;
    seq_t sum = (seq_t)9 + (seq_t)7 * n_seq + (seq_t)2 * n_seq * n_seq;

    return pow * sum;
}
