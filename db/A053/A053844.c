/*
 * A053844  (Sum of digits of n written in base 9) modulo 9.
 * https://oeis.org/A053844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (sum of digits of n written in base 9) modulo 9
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Accumulate the digit sum modulo 9 while converting n to base 9. */
    seq_t sum = 0;
    while (n > 0) {
        sum = (sum + (seq_t)(n % 9)) % 9;
        n /= 9;
    }
    return sum;
}
