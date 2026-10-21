/*
 * A215285  Numbers m such that Sum_{k=1..m} (m - k | k) = phi(m), where (i|j)
 *          is the Kronecker symbol and phi(m) is the Euler totient function.
 * https://oeis.org/A215285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1, 2, 3, 4, 6, 9, 4*(n-5)^2 for n ≥ 7
 * Range:     u8 n <= 12, u16 n <= 132, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset starts at 1; undefined below */
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 3;
    if (n == 4) return 4;
    if (n == 5) return 6;
    if (n == 6) return 9;

    /* for n >= 7, a(n) = 4 * (n-5)^2 */
    idx_t k = n - 5;
    seq_t sk = (seq_t)k;
    seq_t sq = sk * sk;
    return (seq_t)4 * sq;
}
