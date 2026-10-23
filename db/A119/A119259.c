/*
 * A119259  Central terms of the triangle in A119258.
 * https://oeis.org/A119259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} C(n+k-1, k) * 2^k (intended for OEIS
 *            A119259)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum  = 0;
    seq_t comb = 1;          /* comb = C(n+k-1, k), initially k = 0 */
    seq_t pow2 = 1;          /* pow2 = 2^k, initially k = 0 */

    for (idx_t k = 0; k <= n; ++k) {
        if (k != 0) {
            /* Update binomial coefficient using recurrence:
               C(n+k-1, k) = C(n+k-2, k-1) * (n+k-1) / k  */
            comb = (comb * (seq_t)(n + k - 1)) / k;
            pow2 = pow2 * 2;   /* next power of two */
        }
        sum += comb * pow2;
    }
    return sum;
}
