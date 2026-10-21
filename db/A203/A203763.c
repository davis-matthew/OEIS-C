/*
 * A203763  Vandermonde sequence using x^2 - xy + y^2 applied to
 *          (1,1,2,2,...,[n/2]).
 * https://oeis.org/A203763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{1≤i<j≤n} (⌈i/2⌉² - ⌈i/2⌉·⌈j/2⌉ + ⌈j/2⌉²)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        seq_t xi = (seq_t)((i + 1) >> 1);      /* ceil(i/2) */
        for (idx_t j = i + 1; j <= n; ++j) {
            seq_t xj = (seq_t)((j + 1) >> 1);  /* ceil(j/2) */
            seq_t term = xi * xi - xi * xj + xj * xj;
            result *= term;
        }
    }
    return result;
}
