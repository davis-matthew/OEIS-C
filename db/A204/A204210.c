/*
 * A204210  Number of length n+1 nonnegative integer arrays starting and
 *          ending with 0 with adjacent elements differing by no more than 5.
 * https://oeis.org/A204210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+1 arrays starting/ending with 0,
 *            nonnegative, and adjacent elements differ by at most 5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation follows the recurrence:
     dp[0][0] = 1
     dp[i+1][h] = sum_{d=-5..5} dp[i][h+d]      (dp[i][x] = 0 for x < 0)
   and uses a prefix‑sum optimisation to make each step O(h)
   where h = 5*i is the maximal reachable height at that step.
   Results are cached so that calls in arbitrary order are fast. */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/seq_realloc.h"

/* cache for the dynamic programming tables */
static seq_t *dp         = NULL;    /* current state  */
static seq_t *dp_next    = NULL;    /* next state    */
static size_t dp_cap     = 0;       /* allocated size */

static seq_t *pref       = NULL;    /* prefix sums of dp   */
static size_t pref_cap   = 0;       /* allocated size       */

/* memoised answers: result[i] == a(i+1) */
static seq_t *results    = NULL;
static size_t res_cap    = 0;
static size_t res_len    = 0;

/* stored number of steps that have already been computed
   (dp holds the distribution after stored_steps steps) */
static idx_t stored_steps = 0;

/* ensure that dp, dp_next, and pref have enough space for
   height up to 5*n (plus one).  Newly allocated elements are
   initialised to zero. */
static void ensure_capacity(idx_t n)
{
    size_t need = 5 + 5 * (size_t)n;   /* 5*n + 1 */
    if (dp_cap >= need)
        return;

    /* allocate new larger arrays */
    seq_t *new_dp      = alloc_seq_array(need);
    seq_t *new_dp_next = alloc_seq_array(need);
    seq_t *new_pref    = alloc_seq_array(need);

    /* copy old data */
    for (size_t i = 0; i < dp_cap; ++i) {
        new_dp[i]      = dp[i];
        new_dp_next[i] = dp_next[i];
        new_pref[i]    = pref[i];
    }

    /* free old arrays */
    if (dp)         free_seq_array(dp);
    if (dp_next)    free_seq_array(dp_next);
    if (pref)       free_seq_array(pref);

    dp         = new_dp;
    dp_next    = new_dp_next;
    pref       = new_pref;
    dp_cap     = need;
    pref_cap   = need;
}

/* initialise the DP state for the first call */
static void init_state(void)
{
    if (dp != NULL)
        return;            /* already initialised */

    dp_cap = 10;           /* some small start */
    dp      = alloc_seq_array(dp_cap);
    dp_next = alloc_seq_array(dp_cap);
    pref    = alloc_seq_array(dp_cap);

    for (size_t i = 0; i < dp_cap; ++i) {
        dp[i]      = 0;
        dp_next[i] = 0;
        pref[i]    = 0;
    }
    dp[0] = 1;             /* start at height 0 */
    stored_steps = 0;
}

seq_t a(idx_t n)
{
    if (n < 1)              /* undefined for n < 1 */
        return 0;

    /* return memoised result if already computed */
    if (res_len >= (size_t)n)
        return results[(size_t)n - 1];

    /* initialise DP state on first call */
    init_state();

    /* extend tables to cover height up to 5*n */
    ensure_capacity(n);

    /* compute steps up to n */
    for (idx_t step = stored_steps + 1; step <= n; ++step) {
        size_t prev_max = 5 * (size_t)(step - 1);   /* max height after previous step */
        size_t cur_max  = 5 * (size_t)step;         /* max height after current step */

        /* prefix sums of dp over 0..prev_max */
        pref[0] = dp[0];
        for (size_t i = 1; i <= prev_max; ++i)
            pref[i] = pref[i - 1] + dp[i];

        /* compute next state */
        for (size_t h = 0; h <= cur_max; ++h) {
            size_t right = (h + 5 <= prev_max) ? h + 5 : prev_max;
            seq_t sum = pref[right];
            if (h >= 6)                       /* subtract left part of the window */
                sum -= pref[h - 6];
            dp_next[h] = sum;
        }

        /* zero remaining entries beyond cur_max (they may contain garbage) */
        for (size_t h = cur_max + 1; h < dp_cap; ++h)
            dp_next[h] = 0;

        /* swap dp and dp_next for next iteration */
        seq_t *tmp          = dp;
        dp                  = dp_next;
        dp_next             = tmp;
        stored_steps        = step;

        /* store the answer for this step (a(step) = dp[0]) */
        if (res_len == res_cap) {
            size_t new_cap = res_cap ? res_cap * 2 : 8;
            seq_t *new_res = seq_realloc(results, res_cap, new_cap);
            results = new_res;
            res_cap = new_cap;
        }
        results[res_len++] = dp[0];
    }

    return results[(size_t)n - 1];
}
