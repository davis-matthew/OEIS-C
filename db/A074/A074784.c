/*
 * A074784  a(n) = a(n-1) + square of the sum of digits of n.
 * https://oeis.org/A074784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=1..n} (sum of decimal digits of k)^2, a(0) = 0
 * Range:     u8 n <= 8, u16 n <= 486, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k) {
        unsigned int digit_sum = 0;
        idx_t x = k;
        while (x) {
            digit_sum += (unsigned int)(x % 10);
            x /= 10;
        }
        total += (seq_t)(digit_sum * digit_sum);
    }
    return total;
}
