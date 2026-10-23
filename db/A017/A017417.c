/*
 * A017417  a(n) = (11*n + 2)^5.
 * https://oeis.org/A017417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 2)^5
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 7, u64 n <= 648, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn   = (seq_t)n;
    seq_t base = tn * (seq_t)11 + (seq_t)2;
    seq_t res  = (seq_t)1;
    res *= base;
    res *= base;
    res *= base;
    res *= base;
    res *= base;
    return res;
}
