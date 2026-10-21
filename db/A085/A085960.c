/*
 * A085960  Size of the largest code of length 4 and minimum distance 3 over
 *          an alphabet of size n. This is usually denoted by A_{n}(4,3).
 * https://oeis.org/A085960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2, except a(2)=2, a(6)=34
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 2) return (seq_t)2;
    if (n == 6) return (seq_t)34;
    seq_t tn = (seq_t)n;
    return tn * tn;
}
