/*
 * A195159  Multiples of 7 and odd numbers interleaved.
 * https://oeis.org/A195159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(2n) = 7n, a(2n+1) = 2n+1 (OEIS A195159)
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is even, divide by 2 (to get n/2) and multiply by 7.
       If n is odd, the value is just n.                     */
    if ((n & 1) == 0) {
        return ((seq_t)(n >> 1)) * 7;
    } else {
        return (seq_t)n;
    }
}
