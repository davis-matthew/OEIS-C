/*
 * A200984  Number of nX2 0..4 arrays with every row and column nondecreasing
 *          rightwards and downwards, and the number of instances of each
 *          value within one of each other.
 * https://oeis.org/A200984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 35, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A200984: number of n×2 arrays of 0..4 with rows and columns nondecreasing
 *  and the frequency of each value differing by at most one.
 *
 *  Computation method:
 *    - Total entries = 2n, let k = floor(2n/5) and r = 2n mod 5.
 *    - For each choice of r values that occur k+1 times (others occur k times),
 *      we obtain the total multiset counts_t[0..4].
 *    - The arrays are uniquely determined by the counts of each value in
 *      the first column (a) and the second column (b), both sorted
 *      nondecreasingly.
 *    - For a fixed total counts_t, let aCnt[v] = number of value v in first column.
 *      Then bCnt[v] = counts_t[v] - aCnt[v].
 *    - The condition a_i ≤ b_i for all rows is equivalent to
 *      for every v:  Σ_{u≤v} aCnt[u] ≥ Σ_{u≤v} bCnt[u].
 *      This gives the cumulative lower bounds:
 *        cum_a(v) ≥ ceil( cum_counts_t(v) / 2 ).
 *    - Count the integer vectors aCnt satisfying the bounds via a
 *      5‑step DP over the 5 distinct values.
 *
 *  Complexity:  O(5·n) per call, well within 2 s for n ≤ 10000.
 *  The result fits into seq_t; arbitrary‑precision or 64‑bit builds are both
 *  supported.
 */

#include <stdlib.h>

static int bitcount(int x) {
    int c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* offset starts at 1 */

    /* 2n, floor division by 5, remainder */
    idx_t two_n = (idx_t)2 * n;
    idx_t k = two_n / 5;
    idx_t r = two_n % 5;

    /* allocate temporary DP arrays of size n+1 */
    size_t sz = (size_t)(n + 1);
    seq_t *dp_cur = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *prefix = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp_cur || !dp_next || !prefix) {
        /* allocation failed – return 0 as a safe fallback */
        free(dp_cur); free(dp_next); free(prefix);
        return (seq_t)0;
    }

    seq_t result = (seq_t)0;

    /* iterate over all subsets of size r (values with count k+1) */
    for (int mask = 0; mask < 32; ++mask) {
        if (bitcount(mask) != r) continue;

        /* total counts per value and their cumulative sums */
        idx_t counts[5];
        idx_t cum[5];
        idx_t acc = 0;
        for (int v = 0; v < 5; ++v) {
            idx_t add = (mask >> v) & 1;
            counts[v] = k + add;
            acc += counts[v];
            cum[v] = acc;
        }

        /* initialize DP: only 0 entries in first column are possible before any value */
        for (size_t i = 1; i < sz; ++i) dp_cur[i] = (seq_t)0;
        dp_cur[0] = (seq_t)1;

        /* DP over the 5 values */
        for (int v = 0; v < 5; ++v) {
            /* prefix sums of current DP */
            prefix[0] = dp_cur[0];
            for (size_t i = 1; i < sz; ++i) {
                prefix[i] = prefix[i - 1] + dp_cur[i];
            }

            idx_t lower = (cum[v] + 1) / 2; /* ceil(cum/2) */
            for (size_t s = 0; s < sz; ++s) {
                if (s < lower) {
                    dp_next[s] = (seq_t)0;
                    continue;
                }
                idx_t low_index = (s > (size_t)counts[v]) ? s - (size_t)counts[v] : 0;
                seq_t sum_vals = prefix[s] - (low_index > 0 ? prefix[low_index - 1] : (seq_t)0);
                dp_next[s] = sum_vals;
            }
            /* swap DP arrays for next value */
            seq_t *tmp = dp_cur;
            dp_cur = dp_next;
            dp_next = tmp;
        }
        /* dp_cur[n] holds the number of valid a‑column count allocations
           for this totals assignment */
        result += dp_cur[n];
        /* dp_next will be overwritten in next value iteration, no need to clear */
    }

    free(dp_cur);
    free(dp_next);
    free(prefix);
    return result;
}
