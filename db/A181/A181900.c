/*
 * A181900  a(n) = n * A022998(n).
 * https://oeis.org/A181900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(2*n) = 8*n^2, a(2*n+1) = 1 + 4*n + 4*n^2
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t half = n >> 1;
    if ((n & 1) == 0) {
        seq_t h = (seq_t)half;
        return (seq_t)8 * h * h;
    } else {
        seq_t h = (seq_t)half;
        return (seq_t)1 + (seq_t)4 * h + (seq_t)4 * h * h;
    }
}
