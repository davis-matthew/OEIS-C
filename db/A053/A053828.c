/*
 * A053828  Sum of digits of (n written in base 7).
 * https://oeis.org/A053828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum of digits of n in base 7
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t m = n;
    while (m > 0) {
        sum += (seq_t)(m % 7);
        m /= 7;
    }
    return sum;
}
