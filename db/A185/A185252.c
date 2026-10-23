/*
 * A185252  a(n) = Sum_{k=0..ceiling(n/2)} k*binomial(n,k).
 * https://oeis.org/A185252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * sum_{k=0}^{ceil(n/2)-1} binomial(n-1, k)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 28, u64 n <= 60, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* a(0) = 0 */
    idx_t m = (n + 1) / 2;                 /* ceil(n/2) */
    idx_t N = n - 1;                       /* we need binomial(N,k) */
    seq_t bin = 1; /* binomial(N,0) */
    seq_t sum = 0;
    for (idx_t k = 0; k < m; ++k) {
        if (k > 0) {
            /* compute binomial(N,k) from binomial(N,k-1) */
            seq_t num = bin * (seq_t)(N - k + 1);
            bin = num / (seq_t)k;          /* division is exact */
        }
        sum += bin;                       /* add binomial(N,k) to the sum */
    }
    return (seq_t)n * sum;
}
