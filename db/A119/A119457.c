/*
 * A119457  Triangle read by rows: T(n, 1) = n, T(n, 2) = 2*(n-1) for n>1 and
 *          T(n, k) = T(n-1, k-1) + T(n-2, k-2) for 2 < k <= n.
 * https://oeis.org/A119457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (m - k + 1) * Fib(k+1), with m = ceil((sqrt(8*n+1)-1)/2),
 *            k = n - (m-1)*m/2
 * Range:     u8 n <= 75, u16 n <= 296, u32 n <= 1124, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row number m such that m*(m+1)/2 >= n */
    idx_t m = (idx_t)((sqrt((double)n * 8.0 + 1.0) - 1.0) / 2.0);
    while ((m * (m + 1)) / 2 < n) ++m;

    /* Position inside the row (0‑based) */
    idx_t prev = (m - 1) * m / 2;
    idx_t k = n - prev;          /* k is 1‑based in the row */
    seq_t factor = (seq_t)(m - k + 1);

    /* Compute Fib(k+1) iteratively */
    seq_t f0 = 0;
    seq_t f1 = 1;
    if (k + 1 == 0) return factor * f0;          /* never happens */
    if (k + 1 == 1) return factor * f1;          /* never happens */

    for (idx_t i = 2; i <= k + 1; ++i) {
        seq_t tmp = f0 + f1;
        f0 = f1;
        f1 = tmp;
    }

    return factor * f1;
}
