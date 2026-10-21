/*
 * A204212  Number of length n+1 nonnegative integer arrays starting and
 *          ending with 0 with adjacent elements differing by no more than 7.
 * https://oeis.org/A204212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+1 nonnegative integer arrays starting
 *            and ending with 0 with adjacent elements differing by no more
 *            than 7
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* • a(idx_t n) returns the nth term (1‑based).  The implementation
   builds the sequence incrementally using a dynamic programming
   approach.  For n up to about 10000 the routine terminates well
   under the time limit on typical 64‑bit / 128‑bit integer builds.
*/

seq_t a(idx_t n)
{
    /* ---------- static state ---------------------------------------- */
    static idx_t max_n = 0;           /* highest n that has been computed */
    static seq_t *ans   = NULL;       /* cache of a(k), 1 <= k <= max_n   */
    static size_t ans_cap = 0;        /* allocated capacity of ans          */
    static seq_t *prev   = NULL;      /* dp array for the previous step     */
    static size_t prev_cap = 0;       /* allocated capacity of prev         */

    /* ---------- ensure the result for n is computed ----------------- */
    if (n <= max_n)             /* already known value */
        return ans[n];

    /* ---------- initialization (first run) --------------------------- */
    if (max_n == 0) {
        /* ans[0] unused, allocate space for a(1) */
        ans_cap = 2;
        ans = (seq_t *)malloc(ans_cap * sizeof(seq_t));
        ans[0] = 0;                     /* dummy, not used */
        ans[1] = (seq_t)1;              /* a(1) = 1            */
        max_n  = 1;

        /* prev will hold dp[1][k]  (0 <= k <= 7), all equal to 1 */
        prev_cap = 8;
        prev = (seq_t *)malloc(prev_cap * sizeof(seq_t));
        for (size_t i = 0; i < prev_cap; ++i)
            prev[i] = (seq_t)1;
    }

    /* ---------- grow the cache to accommodate n --------------------- */
    if (ans_cap <= (size_t)n) {
        ans = (seq_t *)realloc(ans, (n + 1) * sizeof(seq_t));
        ans_cap = (size_t)n + 1;
    }

    /* ---------- extend the DP up to n --------------------------------*/
    for (idx_t step = max_n + 1; step <= n; ++step) {
        /* size of the new dp array (positions 0 … 7*step) */
        size_t cur_size   = 7 * step + 1;
        size_t prev_size  = 7 * (step - 1) + 1;

        /* allocate temporary array for the current step */
        seq_t *cur = (seq_t *)malloc(cur_size * sizeof(seq_t));

        /* sliding‑window sum over the previous array */
        size_t low  = 0;
        size_t high = (prev_size > 7 ? 7 : prev_size - 1);
        seq_t window = 0;
        for (size_t j = low; j <= high; ++j)
            window += prev[j];

        /* compute cur[k] = sum_{j=max(0,k-7)}^{min(k+7,prev_size-1)} prev[j] */
        for (size_t k = 0; k < cur_size; ++k) {
            cur[k] = window;

            if (k == cur_size - 1)
                break;                      /* last element, nothing to update */

            size_t new_low  = (k + 1 > 6 ? k + 1 - 7 : 0);
            size_t new_high = (k + 1 + 7 < prev_size ? k + 1 + 7 : prev_size - 1);

            if (new_low > low) {
                for (size_t j = low; j < new_low; ++j)
                    window -= prev[j];
                low = new_low;
            }
            if (new_high > high) {
                for (size_t j = high + 1; j <= new_high; ++j)
                    window += prev[j];
                high = new_high;
            }
        }

        /* store a(step) = cur[0] */
        ans[step] = cur[0];

        /* free the previous array and make current array the new previous */
        free(prev);
        prev = cur;
        prev_cap = cur_size;
    }

    max_n = n;
    return ans[n];
}
