/*
 * A060544  Centered 9-gonal (also known as nonagonal or enneagonal) numbers.
 *          Every third triangular number, starting with a(1)=1.
 * https://oeis.org/A060544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = (3*n - 1)*(3*n - 2)/2
 * Range:     u8 n <= 8, u16 n <= 121, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = 3 * nn - 1;
    seq_t t2 = 3 * nn - 2;
    return (t1 * t2) / 2;
}
