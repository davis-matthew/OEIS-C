/*
 * A158515  Number of colors needed to paint a wheel graph on n nodes.
 * https://oeis.org/A158515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=2, else a(n)=4 if n is even, otherwise 3
 * Range:     u8 n <= 1001, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;
    /* for n >= 3, the wheel graph has chromatic number 4 if n is even,
       otherwise 3. */
    return (n & 1) ? 3 : 4;
}
