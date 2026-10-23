/*
 * A177071  a(n) = (7*n + 3)*(7*n + 4).
 * https://oeis.org/A177071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 3)*(7*n + 4)
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v  = (seq_t)n;
    seq_t f1 = v * 7 + 3;
    seq_t f2 = v * 7 + 4;
    return f1 * f2;
}
