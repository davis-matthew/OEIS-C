/*
 * A016989  a(n) = (7*n)^9.
 * https://oeis.org/A016989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n)^9
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 19, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute (7*n)^9 as 7^9 * n^9, using only integer arithmetic */
    seq_t base = (seq_t)7 * (seq_t)n;   /* base = 7*n */
    seq_t res  = 1;
    /* exponent 9, perform multiplication 9 times */
    for (int i = 0; i < 9; ++i)
        res *= base;
    return res;
}
