/*
 * A014813  a(n) = Sum_{k=0..n} ceiling(k^3/n).
 * https://oeis.org/A014813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} ceil(k^3/n)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t n_seq = (seq_t)n;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t k_seq = (seq_t)k;
        seq_t k3 = k_seq * k_seq * k_seq;        /* k^3 */
        seq_t term = (k3 + n_seq - 1) / n_seq;   /* ceil(k^3 / n) */
        sum += term;
    }
    return sum;
}
