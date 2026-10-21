/*
 * A053838  a(n) = (sum of digits of n written in base 3) modulo 3.
 * https://oeis.org/A053838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (sum of digits of n written in base 3) modulo 3
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    while (n > 0) {
        sum += (seq_t)(n % 3);
        n /= 3;
    }
    return sum % 3;
}
