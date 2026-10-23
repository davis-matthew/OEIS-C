/*
 * A026327  a(n) = number of (s(0), s(1), ..., s(n)) such that s(i) is a
 *          nonnegative integer and |s(i) - s(i-1)| <= 1 for i = 1,2,...,n,
 *          s(0) = 2, s(n) = 4. Also a(n) = T(n,n-2), where T is the array in
 *          A026323.
 * https://oeis.org/A026327
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seq: a(n) = number of step‑by‑step sequences s(0..n) with
 *          s(0)=2, s(n)=4, s(i)≥0, |s(i)-s(i-1)|≤1.
 * This implementation uses a dynamic programming table.
 * The transition counts only valid heights, so no negative indices are used.
 * Complexity: O(n) time & O(n) memory per call. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static idx_t last_computed_step = 0;     /* largest n already processed */
static seq_t *dp_prev = NULL;            /* values after step k-1 */
static seq_t *dp_cur  = NULL;            /* temporary table for step k   */
static seq_t *memo    = NULL;            /* memoized a(n) values         */
static size_t dp_capacity = 0;           /* allocated size of DP tables   */
static size_t memo_capacity = 0;         /* allocated size of memo array  */
static const seq_t ONE = (seq_t)1;

static void ensure_capacity(idx_t target)
{
    /* Maximum height that can be reached after the target steps: 2 + target   */
    size_t needed_dp = (size_t)(target + 5);          /* +5 for safety          */
    size_t needed_memo = (size_t)(target + 1);       /* indices 0..target      */

    /* Resize DP tables if needed */
    if (dp_prev == NULL) {
        dp_prev = (seq_t *)calloc(needed_dp, sizeof(seq_t));
        dp_cur  = (seq_t *)calloc(needed_dp, sizeof(seq_t));
        dp_capacity = needed_dp;
    } else if (dp_capacity < needed_dp) {
        dp_prev = (seq_t *)realloc(dp_prev, needed_dp * sizeof(seq_t));
        dp_cur  = (seq_t *)realloc(dp_cur,  needed_dp * sizeof(seq_t));
        /* Zero the newly added part */
        memset(dp_prev + dp_capacity, 0, (needed_dp - dp_capacity) * sizeof(seq_t));
        memset(dp_cur  + dp_capacity, 0, (needed_dp - dp_capacity) * sizeof(seq_t));
        dp_capacity = needed_dp;
    }

    /* Resize memo array if needed */
    if (memo == NULL) {
        memo = (seq_t *)calloc(needed_memo, sizeof(seq_t));
        memo_capacity = needed_memo;
    } else if (memo_capacity < needed_memo) {
        memo = (seq_t *)realloc(memo, needed_memo * sizeof(seq_t));
        memset(memo + memo_capacity, 0, (needed_memo - memo_capacity) * sizeof(seq_t));
        memo_capacity = needed_memo;
    }
}

static void compute_up_to(idx_t target)
{
    if (target <= last_computed_step)
        return;

    ensure_capacity(target);

    /* Initialise step 0: only state y = 2 has one way */
    if (last_computed_step == 0) {
        dp_prev[2] = ONE;          /* height 2 -> index 2   */
    }

    /* Iterate steps 1 .. target */
    for (idx_t step = last_computed_step + 1; step <= target; ++step) {
        /* Max height after this step: 2 + step   */
        size_t next_len = (size_t)(2 + step + 1);   /* indices 0 .. 2+step */
        size_t prev_len = next_len - 1;            /* indices 0 .. 2+step-1 */

        /* Zero dp_cur for the part that will be used */
        memset(dp_cur, 0, next_len * sizeof(seq_t));

        /* Transition from dp_prev to dp_cur */
        for (size_t idx = 0; idx < prev_len; ++idx) {
            seq_t val = dp_prev[idx];
            if (val == (seq_t)0)
                continue;
            /* stay */
            dp_cur[idx] += val;
            /* move up */
            if (idx + 1 < next_len)
                dp_cur[idx + 1] += val;
            /* move down only if we stay at non‑negative height */
            if (idx > 0)
                dp_cur[idx - 1] += val;
        }

        /* Record a(step): number of ways to be exactly at height 4 */
        memo[step] = dp_cur[4];

        /* Prepare for next iteration: dp_prev becomes dp_cur */
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur  = tmp;
    }

    last_computed_step = target;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* sequence defined for n ≥ 2 only */

    compute_up_to(n);
    return memo[n];
}
