/*
 * A171418  Expansion of (1+x)^4/(1-x).
 * https://oeis.org/A171418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(5, n-2*k)
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t binom5[6] = {1, 5, 10, 10, 5, 1};
    seq_t sum = 0;
    idx_t half = n >> 1;                 /* floor(n/2) */
    for (idx_t k = 0; k <= half; ++k) {
        idx_t m = n - (k << 1);          /* n - 2*k */
        if (m <= 5) {
            sum += binom5[m];
        }
    }
    return sum;
}
