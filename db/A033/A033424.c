/*
 * A033424  a(n) = floor(10^8/n).
 * https://oeis.org/A033424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(10^8/n)
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset 1, but guard against divide by zero */
    return (seq_t)100000000 / (seq_t)n;
}
