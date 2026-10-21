/*
 * A203774  Square root of v(2n)/v(2n-1), where v=A203773.
 * https://oeis.org/A203774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * ∏_{k=1}^{n-1} (k^2 + n^2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*      (a(1) = 1, a(2) = 10, a(3) = 390, ...) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; undefined for n == 0 */
    seq_t result = (seq_t)n;        /* start with the n factor */
    seq_t norm_n = result;
    for (idx_t k = 1; k < n; ++k) {
        seq_t kk = (seq_t)k;          /* cast k to seq_t for multiplication */
        seq_t term = kk * kk + norm_n * norm_n;
        result *= term;
    }
    return result;
}
