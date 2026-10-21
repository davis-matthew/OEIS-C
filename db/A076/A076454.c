/*
 * A076454  Sum of numbers that can be written as t*n + u*(n+1) for
 *          nonnegative integers t,u in exactly one way.
 * https://oeis.org/A076454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(2*n^2-1)/2
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 254, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast n to seq_t once to avoid 64‑bit overflow during multiplication */
    seq_t sn = (seq_t)n;
    seq_t t1 = sn * (sn + 1);          /* n*(n+1)   */
    seq_t t2 = 2 * sn * sn - 1;        /* 2*n^2-1   */
    seq_t prod = t1 * t2;              /* n*(n+1)*(2*n^2-1) */
    return prod / 2;                   /* divide by 2 */
}
