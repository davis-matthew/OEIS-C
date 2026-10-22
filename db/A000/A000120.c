/*
 * A000120  Ones-counting sequence: number of 1's in binary expansion of n (or
 *          the binary weight of n).
 * https://oeis.org/A000120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, core, nice, hear, look, base
 * Formula:   a(0) = 0, a(2*n) = a(n), a(2*n+1) = a(n) + 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t c = 0;
    while (n) {
        c += (seq_t)(n & 1);
        n >>= 1;
    }
    return c;
}
