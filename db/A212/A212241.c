/*
 * A212241  Number of 2 X 2 matrices M with terms in {1,...,n} such that
 *          permanent(M) > n.
 * https://oeis.org/A212241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 - Σ_{a=1}^{n} Σ_{d=1}^{⌊n/a⌋} F[n - a·d] where F[t]
 *            counts pairs (b,c) with 1≤b,c≤n and b·c ≤ t. This counts the 2×2
 *            matrices with entries 1..n and permanent > n
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Allocate array to hold frequencies of products p = b*c (<= n)
       and later cumulative counts F[t] = #{(b,c): b*c ≤ t}   */
    seq_t *cum = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!cum) return 0;          /* fallback on allocation failure */

    /* initialize to 0 */
    for (idx_t i = 0; i <= n; ++i) cum[i] = 0;

    /* Count exact products b*c <= n */
    for (idx_t b = 1; b <= n; ++b) {
        idx_t max_c = n / b;               /* floor(n / b) */
        for (idx_t c = 1; c <= max_c; ++c) {
            idx_t p = b * c;               /* p <= n */
            cum[p]++;                      /* frequency of product p */
        }
    }

    /* Convert to cumulative counts F[t] = Σ_{p<=t} freq[p] */
    for (idx_t t = 1; t <= n; ++t) {
        cum[t] += cum[t - 1];
    }

    /* Count matrices with permanent ≤ n */
    seq_t bound = 0;
    for (idx_t a = 1; a <= n; ++a) {
        idx_t max_d = n / a;               /* a·d <= n */
        for (idx_t d = 1; d <= max_d; ++d) {
            idx_t p = a * d;               /* product of a and d */
            idx_t limit = n - p;           /* remaining allowed product for b·c */
            if (limit > 0) {
                bound += cum[limit];
            }
        }
    }

    /* Total number of 2×2 matrices with entries in {1,…,n} is n^4 */
    seq_t nn = (seq_t)n;
    seq_t total = nn * nn;           /* n^2 */
    total *= nn * nn;                /* n^4 */

    /* Free temporary array */
    free(cum);

    return total - bound;            /* number with permanent > n */
}
