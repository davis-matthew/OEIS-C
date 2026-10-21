/*
 * A057147  a(n) = n times sum of digits of n.
 * https://oeis.org/A057147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = n * sum_of_digits(n)
 * Range:     u8 n <= 27, u16 n <= 2688, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t t = n;
    seq_t sum = 0;
    while (t > 0) {
        sum += t % 10;
        t /= 10;
    }
    return (seq_t)n * sum;
}
