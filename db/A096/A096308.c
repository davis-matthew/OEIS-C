/*
 * A096308  a(n) = Sum_{d divides n} (-1)^(n-d)*Stirling1(n,d).
 * https://oeis.org/A096308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d|n} (-1)^(n-d) * Stirling1(n,d)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*              = Sum_{d|n} |Stirling1(n,d)|  (unsigned Stirling numbers of the first kind) */

#include <stddef.h>
#include <stdlib.h>

/*  seq_t and idx_t are provided by the build environment.  */
seq_t a(idx_t n)
{
    /* Static cache of previously computed values.  */
    static seq_t *cache = NULL;          /* a[0] unused, a[1] … a[max_computed] */
    static size_t cache_len = 0;
    static seq_t *prev  = NULL;          /* previous Stirling row (for n-1) */
    static size_t prev_size = 0;
    static idx_t max_computed = 0;

    /* Initialise the first (empty) Stirling row (n = 0) before any work.  */
    if (!prev) {
        prev       = (seq_t*)malloc(sizeof(seq_t));
        prev[0]    = (seq_t)1;        /* c(0,0) = 1 */
        prev_size  = 1;
    }

    /* Return cached value if already computed. */
    if (n <= max_computed)
        return cache[(size_t)n];

    /* Compute successive rows up to n. */
    for (idx_t cur_n = max_computed + 1; cur_n <= n; ++cur_n) {
        /* Allocate space for the new Stirling row (indices 0 … cur_n). */
        seq_t *cur = (seq_t*)malloc(((size_t)cur_n + 1) * sizeof(seq_t));
        if (!cur)
            return (seq_t)0;          /* allocation failure: return zero */

        cur[0] = (seq_t)0;
        for (idx_t k = 1; k <= cur_n; ++k) {
            size_t ki  = (size_t)k;
            seq_t part1 = 0, part2 = 0;
            if (ki - 1 < prev_size)
                part1 = prev[ki - 1];
            if (ki < prev_size)
                part2 = prev[ki] * ((seq_t)(cur_n - 1));
            cur[ki] = part1 + part2;
        }

        /* Sum the Stirling numbers over all divisors of cur_n. */
        seq_t sum = (seq_t)0;
        for (idx_t d = 1; d * d <= cur_n; ++d) {
            if (cur_n % d == 0) {
                sum += cur[(size_t)d];
                idx_t other = cur_n / d;
                if (other != d)
                    sum += cur[(size_t)other];
            }
        }

        /* Store the computed a(cur_n) value. */
        if (cache_len <= (size_t)cur_n) {
            cache_len = (size_t)cur_n + 1;
            cache = (seq_t*)realloc(cache, cache_len * sizeof(seq_t));
        }
        cache[(size_t)cur_n] = sum;

        /* Prepare for the next iteration: free old previous row and store the new one. */
        free(prev);
        prev       = cur;
        prev_size  = (size_t)cur_n + 1;
    }

    max_computed = n;
    return cache[(size_t)n];
}
