/*
 * A053830  Sum of digits of (n written in base 9).
 * https://oeis.org/A053830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum of digits of (n written in base 9)
 * Range:     u8 n <= 9826, u16 n <= 9826, u32 n <= 9826, u64 n <= 9826,
 *            u128 n <= 9826, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    while (n) {
        sum += (seq_t)(n % 9);
        n /= 9;
    }
    return sum;
}
