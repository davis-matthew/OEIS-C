/*
 * A065039  If n in base 10 is d_1 d_2 ... d_k then a(n) = d_1 + d_1.d_2 +
 *          d_1.d_2.d_3 + ... + d_1. ... .d_k, where the dot means the
 *          concatenation operation.
 * https://oeis.org/A065039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = Sum_{k>=0} floor(n/10^k)
 * Range:     u8 n <= 230, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t t = n;
    while (t > 0) {
        sum += (seq_t)t;
        t /= 10;
    }
    return sum;
}
