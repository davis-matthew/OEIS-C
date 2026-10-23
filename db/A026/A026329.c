/*
 * A026329  a(n) = number of (s(0), s(1), ..., s(n)) such that s(i) is a
 *          nonnegative integer and |s(i) - s(i-1)| <= 1 for i = 1,2,...,n,
 *          s(0) = 2, s(n) = 6. Also a(n) = T(n,n-4), where T is the array in
 *          A026323.
 * https://oeis.org/A026329
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Series: A026329
   a(n) = number of sequences s(0)…s(n) with s(0)=2, s(n)=6,
   0 ≤ s(i), |s(i)-s(i-1)|≤1.
   Computed by dynamic programming on the state space of possible
   levels.  For each step k, dp[i] is the number of ways to reach
   level i after k steps.  The recurrence is
     dp_new[i] = dp[i]          (stay)
                + dp[i-1]       (step up)
                + dp[i+1]       (step down),
   with boundary handling for i=0 and i=max.  a(n) = dp[6] after n steps.
   Values are memoized in a static cache so that subsequent calls
   do not recompute earlier steps.
*/

#include <stdlib.h>

/* external types supplied by the framework */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* cache of computed a(k) values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* state for the dynamic program */
    static seq_t *dp_cur = NULL;
    static seq_t *dp_next = NULL;
    static idx_t dp_size  = 0;   /* number of levels allocated */
    static idx_t cur_n    = 0;   /* last number of steps computed */

    /* lower offset of the sequence */
    const idx_t OFFSET = 4;

    if (n < OFFSET)
        return (seq_t)0;              /* undefined for n < 4 */

    /* Resize the result cache if necessary */
    if (cache_size <= n) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* allocation failure */
        cache = tmp;
        for (size_t i = cache_size; i < new_size; ++i)
            cache[i] = 0;
        cache_size = new_size;
    }

    /* Return cached value if already computed */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* Ensure DP arrays are large enough for level 0 … 2 + n */
    idx_t needed_size = n + 3;   /* 2 + n + 1 for safety */
    if (dp_size < needed_size) {
        seq_t *tmp_cur  = (seq_t *)realloc(dp_cur,  needed_size * sizeof(seq_t));
        seq_t *tmp_next = (seq_t *)realloc(dp_next, needed_size * sizeof(seq_t));
        if (!tmp_cur || !tmp_next) {
            /* fall back to recompute from scratch */
            return (seq_t)0;
        }
        dp_cur  = tmp_cur;
        dp_next = tmp_next;
        for (idx_t i = dp_size; i < needed_size; ++i) {
            dp_cur[i]  = 0;
            dp_next[i] = 0;
        }
        dp_size = needed_size;
    }

    /* Initialize the DP arrays for the very first call */
    if (cur_n == 0) {
        for (idx_t i = 0; i < dp_size; ++i)
            dp_cur[i] = 0;
        dp_cur[2] = 1;    /* start at level 2 */
    }

    /* Simulate steps from cur_n+1 up to n */
    for (idx_t step = cur_n + 1; step <= n; ++step) {
        /* i = 0 */
        dp_next[0] = dp_cur[0];
        if (dp_size > 1)
            dp_next[0] += dp_cur[1];

        /* middle indices */
        for (idx_t i = 1; i + 1 < dp_size; ++i) {
            dp_next[i] = dp_cur[i];
            dp_next[i] += dp_cur[i-1];
            dp_next[i] += dp_cur[i+1];
        }

        /* last index */
        idx_t last = dp_size - 1;
        dp_next[last] = dp_cur[last];
        if (last > 0)
            dp_next[last] += dp_cur[last-1];

        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        /* store result for this step */
        cache[step] = (6 < dp_size) ? dp_cur[6] : (seq_t)0;
        cur_n = step;
    }

    return cache[n];
}
