/*
 * A166359  Diagonal sums of the exponential Riordan array [1+x*arctanh(x),
 *          x], A166357.
 * https://oeis.org/A166359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = \sum_{k=0}^n \binom{n+k}{2k} \cdot \frac{(2k)!}{2k-1},
 *            with the convention that \frac{(2\cdot0)!}{2\cdot0-1}=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;       /* accumulated result */
    seq_t B = 1;         /* \binom{n+k}{2k}, start with k=0 */
    seq_t fact = 1;      /* running factorial */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t d;         /* A166356(k) = (2k)!/(2k-1) */
        if (k == 0) {
            d = 1;
        } else {
            fact *= (seq_t)(2 * k - 1);   /* multiply by (2k-1) */
            fact *= (seq_t)(2 * k);       /* multiply by (2k)   */
            d = fact / (seq_t)(2 * k - 1);/* divide by (2k-1)   */
        }

        sum += B * d;          /* add current term */

        if (k < n) {
            idx_t num = (n + k + 1) * (n - k);
            idx_t den = (2 * k + 2) * (2 * k + 1);
            B = (B * (seq_t)num) / (seq_t)den;   /* next binomial coefficient */
        }
    }

    return sum;
}
