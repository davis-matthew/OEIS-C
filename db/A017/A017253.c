/*
 * A017253  a(n) = (9*n + 7)^9.
 * https://oeis.org/A017253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 7)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 14, u128 n <= 2122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)9 * n + 7;
    seq_t res  = 1;
    /* Multiply base 9 times (exponent is small and fixed) */
    for (idx_t i = 0; i < 9; ++i)
        res *= base;
    return res;
}
