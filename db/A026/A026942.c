/*
 * A026942  a(n) = Sum_{k=0..n-3} T(n,k) * T(n,k+3), with T given by A026300.
 * https://oeis.org/A026942
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-3} T(n,k) * T(n,k+3), T is defined by
 *            T(0,0)=1 and T(n,k)=T(n-1,k-1)+T(n-1,k)+T(n-1,k+1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* static state for incremental computation and caching */
    static idx_t current_n = 0;          /* highest n that has been computed */
    static seq_t *prev = NULL;           /* previous row */
    static seq_t *curr = NULL;           /* current row (temporary) */
    static size_t prev_cap = 0, curr_cap = 0;  /* allocated capacities */

    static seq_t *cache = NULL;          /* cache of a(n) values */
    static size_t cache_cap = 0;         /* allocated size of cache */

    /* initialization on first call */
    if (prev == NULL) {
        prev_cap = 1;
        curr_cap = 1;
        prev = seq_alloc(prev_cap);
        curr = seq_alloc(curr_cap);
        if (!prev || !curr) return (seq_t)0;   /* allocation failure */
        prev[0] = (seq_t)1; /* T(0,0) = 1 */
        cache_cap = 3;              /* indices 0..2 (valid for n<3) */
        cache = seq_alloc(cache_cap);
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = (seq_t)0;
        current_n = 0;
    }

    /* Extend computation up to requested n */
    if (n > current_n) {
        for (idx_t r = current_n + 1; r <= n; ++r) {
            /* Ensure curr has capacity r+1 */
            if (curr_cap < (size_t)(r + 1)) {
                seq_t *tmp = seq_realloc(curr, curr_cap, (size_t)(r + 1));
                if (!tmp) return (seq_t)0;          /* allocation failure */
                curr = tmp;
                curr_cap = (size_t)(r + 1);
            }

            /* Compute row r based on previous row (prev) */
            for (idx_t k = 0; k <= r; ++k) {
                seq_t s = 0;
                if (k < r)          s += prev[k];       /* T(r-1,k)   */
                if (k > 0)          s += prev[k - 1];   /* T(r-1,k-1) */
                if (k + 1 < r)      s += prev[k + 1];   /* T(r-1,k+1) */
                curr[k] = s;
            }

            /* Compute a(r) if r >= 3 */
            if (r >= 3) {
                seq_t sum = 0;
                for (idx_t k = 0; k <= r - 3; ++k) {
                    sum += curr[k] * curr[k + 3];
                }

                /* Extend cache if necessary */
                if ((size_t)(r + 1) > cache_cap) {
                    seq_t *tmpc = seq_realloc(cache, cache_cap, (size_t)(r + 1));
                    if (!tmpc) return (seq_t)0;         /* allocation failure */
                    /* Zero newly allocated part */
                    for (size_t i = cache_cap; i < (size_t)(r + 1); ++i)
                        tmpc[i] = (seq_t)0;
                    cache = tmpc;
                    cache_cap = (size_t)(r + 1);
                }
                cache[r] = sum;
            }

            /* Prepare for next iteration: swap rows */
            seq_t *tmp = prev;
            prev = curr;
            curr = tmp;

            size_t tmp_cap = prev_cap;
            prev_cap = curr_cap;
            curr_cap = tmp_cap;

            current_n = r;
        }
    }

    /* Return cached value (valid for n >= 0 after first call) */
    if ((size_t)n < cache_cap) return cache[n];
    return (seq_t)0;
}
