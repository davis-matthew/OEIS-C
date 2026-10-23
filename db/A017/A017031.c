/*
 * A017031  a(n) = (7*n + 4)^3.
 * https://oeis.org/A017031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 4)^3
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 231, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t = (seq_t)7 * nn + (seq_t)4;
    seq_t t2 = t * t;
    return t2 * t;
}
