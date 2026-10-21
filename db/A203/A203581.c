/*
 * A203581  Vandermonde sequence using x^2 + xy + y^2 applied to
 *          (2,3,...,n+1).
 * https://oeis.org/A203581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{2<=x<y<=n+1} (x^2 + x*y + y^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                     /* defined for offset < 1 */

    seq_t result = 1;
    for (idx_t y = 2; y <= n + 1; ++y) {
        seq_t y_sq = (seq_t)y * (seq_t)y;
        for (idx_t x = 2; x < y; ++x) {
            seq_t term = (seq_t)x * (seq_t)x + (seq_t)x * (seq_t)y + y_sq;
            result *= term;
        }
    }
    return result;
}
