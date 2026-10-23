/*
 * A195312  Multiples of 9 and odd numbers interleaved.
 * https://oeis.org/A195312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = (7*(-1)^n + 11)*n/4
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        /* n is even: a(n) = 9*n/2 */
        return ((seq_t)n * 9) / 2;
    } else {
        /* n is odd:  a(n) = n */
        return (seq_t)n;
    }
}
