/*
 * A195140  Multiples of 5 and odd numbers interleaved.
 * https://oeis.org/A195140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 5*(n/2) if n even, else n
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {               /* n is even: a(2k) = 5k */
        return ((seq_t)(n / 2)) * 5;
    } else {                           /* n is odd: a(2k+1) = 2k+1 = n */
        return (seq_t)n;
    }
}
