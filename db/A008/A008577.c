/*
 * A008577  Crystal ball sequence for planar net 4.8.8.
 * https://oeis.org/A008577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor(4 * (n² + n + 1) / 3)
 * Range:     u8 n <= 7, u16 n <= 165, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t = nn * nn + nn + 1;   /* n² + n + 1 */
    t   = t * 4;                  /* 4*(n² + n + 1) */
    return t / 3;                 /* integer division = floor for unsigned */
}
