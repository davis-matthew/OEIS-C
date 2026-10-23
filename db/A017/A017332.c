/*
 * A017332  a(n) = (10*n + 5)^4.
 * https://oeis.org/A017332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 5)^4
 * Range:     u8 none, u16 n <= 1, u32 n <= 25, u64 n <= 6553, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (10*n + 5)^4 using only seq_t arithmetic. */
    seq_t base = (seq_t)n * 10 + 5;   /* 10*n + 5 */
    seq_t base2 = base * base;        /* (10*n + 5)^2 */
    return base2 * base2;             /* (10*n + 5)^4 */
}
