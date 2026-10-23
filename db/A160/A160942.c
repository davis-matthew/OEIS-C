/*
 * A160942  a(n)= n * digital sum(n-1).
 * https://oeis.org/A160942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n * digit_sum(n-1)
 * Range:     u8 n <= 28, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t x = n - 1;
    while (x) {
        sum += (seq_t)(x % 10);
        x /= 10;
    }
    return (seq_t)n * sum;
}
