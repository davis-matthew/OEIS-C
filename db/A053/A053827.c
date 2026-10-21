/*
 * A053827  Sum of digits of (n written in base 6).
 * https://oeis.org/A053827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum of the digits of n written in base 6
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
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
        sum += (seq_t)(t % 6);
        t /= 6;
    }
    return sum;
}
