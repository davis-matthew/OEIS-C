/*
 * A200759  Number of 0..3 arrays x(0..n-1) of n elements with nondecreasing
 *          average value.
 * https://oeis.org/A200759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n arrays over {0,1,2,3} with
 *            nondecreasing average. Computed by dynamic programming. Initial
 *            terms: a(1)=4, a(2)=10, a(3)=23, …
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 30, u64 n <= 63, u128 n <= 129,
 *            big any n
 * Helpers:   resize_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/resize_seq.h"

/* Static state */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;

static seq_t *dp_cur      = NULL;
static seq_t *dp_next     = NULL;
static size_t dp_cur_cap  = 0;
static size_t dp_next_cap = 0;
static idx_t  cur_len     = 0;   /* number of elements processed */

/* Public function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* defined only for n ≥ 1 */

    /* Initialise state on first call */
    if (dp_cur == NULL) {
        resize_seq(&dp_cur, &dp_cur_cap, 1);
        dp_cur[0] = 1;            /* length 0, sum 0 → 1 array */
        dp_next = NULL;
        cur_len  = 0;
    }

    /* Ensure cache large enough for requested index */
    if (n > cache_cap)
        resize_seq(&cache, &cache_cap, n);

    /* Compute DP up to desired length n if not already */
    for (idx_t len = cur_len + 1; len <= n; ++len) {
        size_t max_sum_prev = 3 * (len - 1);
        size_t new_cap      = 3 * len + 1;

        resize_seq(&dp_next, &dp_next_cap, new_cap);
        memset(dp_next, 0, new_cap * sizeof(seq_t));

        for (size_t s = 0; s <= max_sum_prev; ++s) {
            seq_t ways = dp_cur[s];
            if (ways == 0)
                continue;

            size_t min_x = 0;
            if (len > 1) {
                /* ceil(s/(len-1)) = (s + (len-2)) / (len-1) */
                min_x = (s + (len - 2)) / (len - 1);
                if (min_x > 3)
                    continue;
            }

            for (size_t x = min_x; x <= 3; ++x) {
                dp_next[s + x] += ways;
            }
        }

        /* Sum all ways to obtain total for this length */
        seq_t total = 0;
        for (size_t i = 0; i < new_cap; ++i)
            total += dp_next[i];

        cache[len - 1] = total;   /* a(len) stored at cache[len-1] */

        /* Prepare for next iteration */
        seq_t *tmp      = dp_cur;
        dp_cur          = dp_next;
        dp_next         = tmp;
        dp_cur_cap      = new_cap;
        dp_next_cap     = dp_cur_cap;   /* both capacities equal now */
        cur_len         = len;
    }

    return cache[n - 1];
}
