/*
 * A017016  a(n) = (7*n + 2)^12.
 * https://oeis.org/A017016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 2)^12
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 5, u128 n <= 231,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 7 + 2;   /* (7*n + 2) */
    seq_t res = 1;
    /* Multiply base by itself 12 times */
    for (int i = 0; i < 12; ++i)
        res *= base;
    return res;
}
