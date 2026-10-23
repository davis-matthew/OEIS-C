/*
 * A024920  a(n) = Sum_{k=1..n} (n-k) * floor(n/k).
 * https://oeis.org/A024920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (n-k) * floor(n/k)
 * Range:     u8 n <= 11, u16 n <= 125, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t q  = (seq_t)n / (seq_t)k;          /* floor(n/k)  */
        seq_t diff = (seq_t)(n - k);             /* n - k       */
        sum += diff * q;
    }
    return sum;
}
