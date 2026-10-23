/*
 * A100795  n occurs n times, as early as possible subject to the constraint
 *          that no two successive terms are identical.
 * https://oeis.org/A100795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1; for n>1, a(n) is the smallest positive integer k such
 *            that k≠a(n‑1) and k has appeared fewer than k times among
 *            a(1)…a(n‑1). This implements the definition of A100795 by
 *            maintaining the remaining count of each integer
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* defined only for n≥1 */

    /* Compute the maximum k that might be needed up to index n.
       Let m be the smallest integer with m(m+1)/2 ≥ n;
       then the largest value that can appear is m+1. */
    idx_t m = 0;
    while (m * (m + 1) / 2 < n) ++m;
    idx_t max_k = m + 1;                 /* inclusive maximum k */

    /* Allocate array for remaining counts, indexed from 1. */
    size_t sz = (size_t)max_k + 3;
    idx_t *rem = (idx_t *)calloc(sz, sizeof(idx_t));
    if (!rem) return 0;                  /* allocation failure */

    for (idx_t k = 1; k <= max_k; ++k)   /* each integer k must appear k times */
        rem[k] = k;

    idx_t prev = 0;                      /* a(0) is imaginary, so 0 is not a valid term */
    seq_t result = 0;

    for (idx_t pos = 1; pos <= n; ++pos) {
        /* Find the smallest admissible integer k. */
        idx_t k = 1;
        while (1) {
            if (k == prev)        ++k;          /* no two successive terms identical */
            else if (rem[k] > 0)  break;        /* k still has remaining slots   */
            else                  ++k;
            /* The loop is guaranteed to terminate because max_k is large
               enough for any n up to 10000 (in fact, for any n). */
        }

        /* Use one occurrence of k. */
        rem[k]--;
        prev = k;

        if (pos == n) result = (seq_t)k;
    }

    free(rem);
    return result;
}
