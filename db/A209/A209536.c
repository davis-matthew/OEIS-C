/*
 * A209536  Number of partitions of 0 having positive part-sum <= n.
 * https://oeis.org/A209536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n} p(j)^2, where p(j) is the partition function
 *            (number of partitions of j). The partition numbers are generated
 *            via Euler's pentagonal recurrence
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 39, u64 n <= 120, u128 n <= 398,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for partition numbers p[0..n] */
    seq_t *p = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!p) return (seq_t)0;          /* allocation failed */

    p[0] = (seq_t)1;                  /* p(0) = 1 */
    for (idx_t i = 1; i <= n; ++i)
        p[i] = (seq_t)0;              /* initialise remainder to 0 */

    seq_t acc = (seq_t)0;             /* running sum of squares */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t sum = (seq_t)0;

        /* Euler's pentagonal recurrence:   p(k) = Σ (-1)^{m-1}
           [ p(k - g1) + p(k - g2) ] where g1 = m(3m-1)/2, g2 = m(3m+1)/2 */
        for (idx_t m = 1; ; ++m) {
            idx_t g1 = m * (3 * m - 1) / 2;
            idx_t g2 = m * (3 * m + 1) / 2;

            if (g1 > k && g2 > k) break;

            if (m & 1) {                     /* odd m  → add */
                if (g1 <= k) sum += p[k - g1];
                if (g2 <= k) sum += p[k - g2];
            } else {                         /* even m → subtract */
                if (g1 <= k) sum -= p[k - g1];
                if (g2 <= k) sum -= p[k - g2];
            }
        }

        p[k] = sum;                          /* partition number p(k) */
        acc += sum * sum;                    /* add square to total */
    }

    free(p);
    return acc;
}
