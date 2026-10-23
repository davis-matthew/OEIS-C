/*
 * A026087  a(n) = number of (s(0), s(1), ..., s(n)) such that every s(i) is
 *          an integer, s(0) = 0, |s(i) - s(i-1)| = 1 for i = 1,2,3; |s(i) -
 *          s(i-1)| <= 1 for i >= 4, s(n) = 4. Also a(n) = T(n,n-4), where T
 *          is the array defined in A026082.
 * https://oeis.org/A026087
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer sequences (s(0)…s(n)) with s(0)=0,
 *            |s(i)-s(i‑1)|=1 for 1≤i≤3, |s(i)-s(i‑1)|≤1 for i≥4, and s(n)=4.
 *            The computation uses a dynamic‑programming recurrence
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, realloc */

static seq_t *dp_prev = NULL;   /* DP array for current step   */
static seq_t *dp_curr = NULL;   /* DP array for next step      */
static long long dp_offset = 0;  /* offset so position p maps to index p+offset  */
static size_t dp_capacity = 0;   /* allocated size of dp arrays           */
static idx_t  max_comp = 0;      /* largest n that has been fully computed */

static seq_t *results = NULL;    /* a(n) values for 0..n, with results[n] valid if n>=4 */
static size_t res_cap = 0;       /* current allocated capacity for results */

/* ensure dp arrays can hold positions up to n (inclusive) */
static void ensure_dp_capacity(idx_t n)
{
    if (dp_prev == NULL) {
        /* first allocation */
        dp_offset = (long long)n;
        dp_capacity = (size_t)(2 * n + 1);
        dp_prev = (seq_t *)malloc(dp_capacity * sizeof(seq_t));
        dp_curr = (seq_t *)malloc(dp_capacity * sizeof(seq_t));
        for (size_t i = 0; i < dp_capacity; ++i) {
            dp_prev[i] = 0;
            dp_curr[i] = 0;
        }
        /* start state: step 0, position 0 */
        dp_prev[(size_t)dp_offset] = 1;
        max_comp = 0;
        return;
    }

    if (n <= dp_offset) return;   /* already enough capacity */

    /* enlarge arrays */
    long long old_offset = dp_offset;
    size_t old_capacity = dp_capacity;
    long long new_offset = (long long)n;
    size_t new_capacity = (size_t)(2 * n + 1);

    seq_t *new_prev = (seq_t *)malloc(new_capacity * sizeof(seq_t));
    seq_t *new_curr = (seq_t *)malloc(new_capacity * sizeof(seq_t));
    for (size_t i = 0; i < new_capacity; ++i) {
        new_prev[i] = 0;
        new_curr[i] = 0;
    }

    /* copy existing DP state into new arrays */
    /* existing data ranges positions -max_comp .. max_comp */
    for (int p = -(int)max_comp; p <= (int)max_comp; ++p) {
        size_t old_idx = (size_t)((long long)p + old_offset);
        size_t new_idx = (size_t)((long long)p + new_offset);
        new_prev[new_idx] = dp_prev[old_idx];
    }

    free(dp_prev);
    free(dp_curr);
    dp_prev = new_prev;
    dp_curr = new_curr;
    dp_offset = new_offset;
    dp_capacity = new_capacity;
}

/* ensure results array can hold index n */
static void ensure_res_capacity(idx_t n)
{
    if (res_cap > n) return;
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_res = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
    if (new_res != results) {
        results = new_res;
    }
    for (size_t i = res_cap; i < new_cap; ++i) {
        results[i] = 0;
    }
    res_cap = new_cap;
}

/* extend DP computation up to step n */
static void extend_to(idx_t n)
{
    if (n <= max_comp) return;             /* already computed */

    ensure_dp_capacity(n);
    ensure_res_capacity(n);

    /* compute steps from max_comp+1 to n */
    for (idx_t step = max_comp + 1; step <= n; ++step) {
        int step_int = (int)step;

        /* clear dp_curr for positions -step .. step */
        for (int p = -step_int; p <= step_int; ++p) {
            size_t idx = (size_t)((long long)p + dp_offset);
            dp_curr[idx] = 0;
        }

        /* transitions from dp_prev (step-1) to dp_curr (step) */
        int prev_start = -(int)(step_int - 1);
        int prev_end   =  (int)(step_int - 1);
        for (int p = prev_start; p <= prev_end; ++p) {
            size_t prev_idx = (size_t)((long long)p + dp_offset);
            seq_t val = dp_prev[prev_idx];
            if (val == 0) continue;

            if (step_int <= 3) {   /* only ±1 moves */
                size_t idx1 = (size_t)((long long)(p - 1) + dp_offset);
                size_t idx2 = (size_t)((long long)(p + 1) + dp_offset);
                dp_curr[idx1] += val;
                dp_curr[idx2] += val;
            } else {              /* ±1 and 0 moves */
                size_t idx1 = (size_t)((long long)(p - 1) + dp_offset);
                size_t idx2 = (size_t)((long long)(p)   + dp_offset);
                size_t idx3 = (size_t)((long long)(p + 1) + dp_offset);
                dp_curr[idx1] += val;
                dp_curr[idx2] += val;
                dp_curr[idx3] += val;
            }
        }

        /* swap dp_prev and dp_curr for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;

        /* store result for this step if step >= 4 */
        if (step_int >= 4) {
            size_t idx_target = (size_t)((long long)4 + dp_offset);
            results[step] = dp_prev[idx_target];
        }
    }

    max_comp = n;
}

/* public function for OEIS A026087 */
seq_t a(idx_t n)
{
    if (n < 4) return 0;
    extend_to(n);
    return results[n];
}
