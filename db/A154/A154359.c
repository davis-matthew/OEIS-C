/*
 * A154359  a(n) = 1250*n^2 - 700*n + 99.
 * https://oeis.org/A154359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1250*n^2 - 700*n + 99
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 1853, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t res = (seq_t)1250 * x;   /* 1250*n */
    res = res * x;                 /* 1250*n^2 */
    res = res - (seq_t)700 * x;    /* -700*n */
    res = res + (seq_t)99;         /* +99 */
    return res;
}
