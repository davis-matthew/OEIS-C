/*
 * A189043  For all permutations of [1..n]: number of distinct values taken by
 *          sum(k=1..n, k^2 * pi(k) ).
 * https://oeis.org/A189043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   for n >= 8 * a(n) = n*(n+1)^2*(n-1)/6 - 9 * (obtained from the
 *            observation that for n >= 8 all sums in the * interval
 *            [n(n+1)^2(n-1)/6, n^2(n+1)^2/4] are attainable except for *
 *            exactly 10 values). For n = 1..7 the sequence values are * 1, 2,
 *            6, 23, 89, 232, 437 respectively
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t small[8] = {0, 1, 2, 6, 23, 89, 232, 437};
    if (n < 8) return small[n];

    seq_t N   = (seq_t)n;
    seq_t t   = N * (N + 1);
    t = t * (N + 1);          /* t = n*(n+1)^2 */
    t = t * (N - 1);          /* t = n*(n+1)^2*(n-1) */
    t = t / 6;                /* divide by 6 */
    return t - 9;             /* subtract the 10 unattained values - 1 */
}
