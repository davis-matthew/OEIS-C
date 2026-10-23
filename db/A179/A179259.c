/*
 * A179259  Arises in covering a graph by forests and a matching.
 * https://oeis.org/A179259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+2)*(3*n^2+1)
 * Range:     u8 n <= 3, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t temp = (seq_t)3 * ns * ns + (seq_t)1;
    seq_t res = (ns + (seq_t)2) * temp;
    return res;
}
