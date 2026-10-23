/*
 * A046895  Sizes of successive clusters in Z^4 lattice.
 * https://oeis.org/A046895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 + 8 * Σ_{d=1}^{n} d * ⌊ n / d ⌋ , counted only for d ≠
 *            0 mod 4 (i.e. 4∤d). This equals the number of integer points in
 *            ℤ⁴ with x² + y² + z² + u² ≤ n
 * Range:     u8 n <= 6, u16 n <= 114, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (d & 3) {                /* d % 4 != 0 */
            idx_t q = n / d;
            sum += (seq_t)d * (seq_t)q;
        }
    }
    return (seq_t)1 + (seq_t)8 * sum;
}
