/*
 * A076458  Sum of numbers that can be written as t*n + u*(n+1) for
 *          nonnegative integers t,u in exactly five ways.
 * https://oeis.org/A076458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)*(10*n^2+8*n-1)/2
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 169, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* outside the defined offset, but handles gracefully */
    seq_t sn  = (seq_t)n;
    seq_t sn1 = sn + 1;
    /* n*(n+1) */
    seq_t part1 = sn * sn1;
    /* n^2 */
    seq_t n2 = sn * sn;
    /* 10*n^2 + 8*n - 1 */
    seq_t poly = 10 * n2 + 8 * sn - 1;
    /* full product */
    seq_t prod = part1 * poly;
    /* division by 2 (exact for all n>=1) */
    return prod / 2;
}
