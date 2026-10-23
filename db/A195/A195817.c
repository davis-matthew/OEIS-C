/*
 * A195817  Multiples of 10 and odd numbers interleaved.
 * https://oeis.org/A195817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = (n%2==0) ? 5*n : n (i.e., (2*(-1)^n+3)*n)
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)n;
    if ((n & 1) == 0) {
        res = res * 5;
    }
    return res;
}
