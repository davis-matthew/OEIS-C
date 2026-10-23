/*
 * A186863  Number of 4-step king's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A186863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 368*n^2 - 1308*n + 1108 for n>2; a(1)=0, a(2)=24
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)24;
    seq_t nn = (seq_t)n;
    return 368 * nn * nn - 1308 * nn + (seq_t)1108;
}
