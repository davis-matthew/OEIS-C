/*
 * A034265  a(n) = binomial(n+6,6)*(6*n+7)/7.
 * https://oeis.org/A034265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+6,6)*(6*n+7)/7
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 58, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t res = 1;
    for (seq_t i = 1; i <= 6; ++i)
    {
        seq_t add = (seq_t)(7 - i);          /* n + 7 - i */
        seq_t term = n_seq + add;
        res = res * term;
        res = res / i;
    }
    seq_t mul = (seq_t)6 * n_seq + (seq_t)7;  /* (6*n + 7) */
    seq_t temp = res * mul;
    return temp / (seq_t)7;
}
