/*
 * A017175  a(n) = (9*n + 1)^3.
 * https://oeis.org/A017175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 1)^3
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 180, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    t = t * 9 + 1;
    return t * t * t;
}
