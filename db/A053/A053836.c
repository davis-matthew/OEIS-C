/*
 * A053836  Sum of digits of n written in base 16.
 * https://oeis.org/A053836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(0)=0, a(16*n+i)=a(n)+i for 0<=i<16
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
        sum += (seq_t)(n & 15); /* add the lowest hexadecimal digit */
        n >>= 4;                /* shift to process the next digit */
    }
    return sum;
}
