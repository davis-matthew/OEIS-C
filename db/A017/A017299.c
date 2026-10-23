/*
 * A017299  a(n) = (10*n + 2)^7.
 * https://oeis.org/A017299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 2)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 56, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * 10 + 2;
    seq_t r = x * x * x * x * x * x * x; /* raise to the 7th power */
    return r;
}
