/*
 * A186864  Number of 5-step king's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A186864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2336*n^2 - 10456*n + 11160 for n > 3 special cases: a(1)
 *            = 0, a(2) = 0, a(3) = 1208 Offset: 1
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 1358, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) return 0;
    if (n == 3) return 1208;
    seq_t nn = (seq_t)n;
    seq_t term1 = (seq_t)2336 * nn * nn + (seq_t)11160;
    seq_t term2 = (seq_t)10456 * nn;
    return term1 - term2;
}
