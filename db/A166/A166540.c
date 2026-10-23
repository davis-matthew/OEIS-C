/*
 * A166540  Number of ways to place 2 nonattacking kings on an n X n X n
 *          raumschach board.
 * https://oeis.org/A166540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 - (3*n-2)^3)/2 for n>0 and a(0)=0
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 44, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t nn = (seq_t)n;
    seq_t n6 = nn * nn * nn * nn * nn * nn;
    seq_t t  = (seq_t)3 * nn - (seq_t)2;
    seq_t t3 = t * t * t;
    seq_t diff = n6 - t3;
    return diff >> 1;
}
