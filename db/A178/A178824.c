/*
 * A178824  a(n) = Sum_{k=0..n} binomial(n,k)^4/(n+1).
 * https://oeis.org/A178824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^4/(n+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use extended 128‑bit arithmetic when seq_t fits in 64 bits.
       For larger seq_t types (128‑bit or arbitrary precision) use
       the builtin seq_t operations.  */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned __int128 sum = 0;
        unsigned __int128 binom = 1;   /* binomial(n,0) */

        for (idx_t k = 0; k <= n; ++k) {
            unsigned __int128 t = binom;
            t *= t;           /* t = binom^2 */
            t *= t;           /* t = binom^4 */
            sum += t;

            if (k < n) {
                binom = binom * (unsigned __int128)(n - k) /
                         (unsigned __int128)(k + 1);
            }
        }
        sum /= (unsigned __int128)(n + 1);
        return (seq_t)sum;
    } else {
        seq_t sum = 0;
        seq_t binom = 1;   /* binomial(n,0) */

        for (idx_t k = 0; k <= n; ++k) {
            seq_t t = binom;
            t *= t;           /* t = binom^2 */
            t *= t;           /* t = binom^4 */
            sum += t;

            if (k < n) {
                binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
            }
        }
        seq_t denom = (seq_t)n + 1;
        return sum / denom;
    }
}
