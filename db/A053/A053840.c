/*
 * A053840  (Sum of digits of n written in base 5) modulo 5.
 * https://oeis.org/A053840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum of digits of n written in base 5, modulo 5
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    while (n) {
        sum += (seq_t)(n % 5);
        sum %= 5;      /* keep the sum within [0,4] */
        n /= 5;
    }
    return sum;
}
