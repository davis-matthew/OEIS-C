/*
 * A203012  Vandermonde sequence using x^2 + xy + y^2 applied to (1,2,...,n).
 * https://oeis.org/A203012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{1≤i<j≤n} (i^2 + i*j + j^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t result = 1;
    for (idx_t j = 2; j <= n; ++j) {
        seq_t j_sq = (seq_t)j * (seq_t)j;
        for (idx_t i = 1; i < j; ++i) {
            seq_t term = (seq_t)i * (seq_t)i + (seq_t)i * (seq_t)j + j_sq;
            result *= term;
        }
    }
    return result;
}
