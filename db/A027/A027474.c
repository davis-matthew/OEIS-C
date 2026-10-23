/*
 * A027474  a(n) = 7^(n-2) * C(n,2).
 * https://oeis.org/A027474
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^(n-2) * binom(n,2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    seq_t res = 1;
    for (idx_t i = 0; i < n - 2; ++i)
    {
        res *= (seq_t)7;
    }
    seq_t binom = ((seq_t)n * ((seq_t)(n - 1))) / (seq_t)2;
    return res * binom;
}
