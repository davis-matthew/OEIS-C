/*
 * A136362  Numbers n such that P+n is not irreducible, where P = x^8 - 8*x^6
 *          + 20*x^4 - 16*x^2 + 2.
 * https://oeis.org/A136362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n=1, a(2)=2, a(n)=4*n^2*(n-2)^2-2 for n>2
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    unsigned __int128 tn  = (unsigned __int128) n;
    unsigned __int128 val = 4 * tn * tn * (tn - 2) * (tn - 2) - 2;
    return (seq_t) val;
}
