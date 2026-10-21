/*
 * A069132  Centered 19-gonal numbers.
 * https://oeis.org/A069132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = (19*n*n - 19*n + 2)/2
 * Range:     u8 n <= 4, u16 n <= 71, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t  = (seq_t)19 * nn * nn;
    t -= (seq_t)19 * nn;
    t += (seq_t)2;
    return t / (seq_t)2;
}
