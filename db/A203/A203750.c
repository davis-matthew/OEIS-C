/*
 * A203750  Square root of v(2n)/v(2n-1), where v=A203748.
 * https://oeis.org/A203750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * ∏_{k=1}^{n-1} (k^2 + k*n + n^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    seq_t n_seq = (seq_t)n;
    seq_t n_sq   = n_seq * n_seq;
    for (idx_t k = 1; k < n; ++k)
    {
        seq_t k_seq = (seq_t)k;
        seq_t term   = k_seq * k_seq + k_seq * n_seq + n_sq;
        result *= term;
    }
    result *= n_seq;
    return result;
}
