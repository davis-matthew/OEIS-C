/*
 * A211523  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w+2x=5y.
 * https://oeis.org/A211523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of ordered triples (w,x,y) with w+2x=5y, w,x,y in
 *            {1,…,n}. Computed by summing over y from 1 to n the number of x
 *            such that 1 ≤ w = 5y – 2x ≤ n, where x also lies in [1,n]
 * Range:     u8 n <= 35, u16 n <= 572, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    if (n == 0) return 0;

    for (idx_t y = 1; y <= n; ++y) {
        idx_t S = 5 * y;                       /* S = 5y  */

        /* lower bound for x: max(1, ceil((S-n)/2)) */
        idx_t lower;
        if (S <= n) {
            lower = 1;
        } else {
            lower = (S - n + 1) / 2;           /* ceil((S-n)/2) for positive */
        }

        /* upper bound for x: min(n, floor((S-1)/2)) */
        idx_t upper = (S - 1) / 2;
        if (upper > n) upper = n;

        if (upper >= lower) {
            total += (seq_t)(upper - lower + 1);
        }
    }
    return total;
}
