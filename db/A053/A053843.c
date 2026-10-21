/*
 * A053843  (Sum of digits of n written in base 8) modulo 8.
 * https://oeis.org/A053843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (sum of digits of n written in base 8) modulo 8
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t x = n;
    unsigned sum = 0;
    while (x) {
        sum += (x & 7ULL);   /* add last octal digit (x % 8) */
        x >>= 3;            /* divide by 8 by shifting 3 bits */
    }
    return (seq_t)(sum & 7);
}
