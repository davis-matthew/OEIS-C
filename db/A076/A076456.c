/*
 * A076456  Sum of numbers that can be written as t*n + u*(n+1) for
 *          nonnegative integers t,u in exactly three ways.
 * https://oeis.org/A076456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(6*n^2 + 4*n - 1)/2 (OEIS A076456)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 193, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t first */
    seq_t sn = (seq_t)n;

    /* (n)*(n+1) */
    seq_t t = sn * (sn + 1);

    /* (6*n^2 + 4*n - 1) */
    seq_t s = (seq_t)6 * sn * sn + (seq_t)4 * sn - 1;

    /* Multiply and divide by 2 */
    t = t * s;
    t = t / 2;

    return t;
}
