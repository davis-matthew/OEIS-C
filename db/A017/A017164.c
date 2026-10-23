/*
 * A017164  a(n) = (9*n)^4.
 * https://oeis.org/A017164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n)^4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 28, u64 n <= 7281,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute (9*n)^4 as ((9*n)^2)^2 */
    seq_t t  = (seq_t)n * 9;   /* 9*n */
    seq_t t2 = t * t;          /* (9*n)^2 */
    seq_t t4 = t2 * t2;        /* (9*n)^4 */
    return t4;
}
