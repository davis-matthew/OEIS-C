/*
 * A097362  a(n) = (n+1)/2 if n is odd, n+2 otherwise.
 * https://oeis.org/A097362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)/2 if n is odd, else n+2
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t x = (seq_t)n;
    if ((x & 1) != 0) {
        return (x + 1) / 2;
    } else {
        return x + 2;
    }
}
