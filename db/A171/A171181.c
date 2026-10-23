/*
 * A171181  n followed by (n - sum of digits of n).
 * https://oeis.org/A171181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = n/2 if n is even; a(n) = floor(n/2) –
 *            digit_sum(floor(n/2)) if n is odd
 * Range:     u8 n <= 511, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1U) == 0U) {
        return (seq_t)(n >> 1);
    } else {
        idx_t k = n >> 1;
        seq_t sum = 0;
        idx_t v = k;
        while (v) {
            sum += (seq_t)(v % 10);
            v /= 10;
        }
        return (seq_t)k - sum;
    }
}
