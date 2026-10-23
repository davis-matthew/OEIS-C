/*
 * A010961  Binomial coefficient C(45,n).
 * https://oeis.org/A010961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(45,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t N = 45;
    if (n > N) return (seq_t)0;
    idx_t k = n;
    if (k > N - k) k = N - k;          /* use symmetry to minimize multiplications */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(N - k + i) / (seq_t)i; /* exact division guarantees integer result */
    }
    return res;
}
