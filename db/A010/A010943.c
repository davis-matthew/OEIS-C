/*
 * A010943  Binomial coefficient C(27,n).
 * https://oeis.org/A010943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(27,n)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t N = 27;
    if (n > N) return 0;
    idx_t k = n;
    /* use symmetry C(27,n) = C(27,27-n) */
    if (k > N - k) k = N - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(N - k + i) / i;
    }
    return res;
}
