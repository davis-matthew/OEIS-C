/*
 * A161351  a(n) = n + sum_of_digits(n) + product_of_digits(n).
 * https://oeis.org/A161351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + sum_of_digits(n) + product_of_digits(n)
 * Range:     u8 n <= 178, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t prod = 1;
    idx_t x   = n;
    while (x > 0) {
        idx_t d = x % 10;
        sum += (seq_t)d;
        prod *= (seq_t)d;
        x /= 10;
    }
    return (seq_t)n + sum + prod;
}
