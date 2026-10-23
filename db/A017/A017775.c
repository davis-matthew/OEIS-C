/*
 * A017775  Binomial coefficients C(59,n).
 * https://oeis.org/A017775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(59, n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t N = 59;
    if (n > N)
        return 0;
    seq_t res = 1;
    for (idx_t k = 1; k <= n; ++k)
        res = res * (seq_t)(N - k + 1) / (seq_t)k;
    return res;
}
