/*
 * A072819  Variance of time for a random walk starting at 0 to reach one of
 *          the boundaries at +n or -n for the first time.
 * https://oeis.org/A072819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2*(n^2 - 1)/3
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 282, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1u)
        return 0;

    seq_t sq  = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t prod = sq * (sq - 1);               /* n^2 * (n^2 - 1) */
    prod /= 3;                                /* division is exact */
    prod *= 2;                                /* multiply by 2 */
    return prod;
}
