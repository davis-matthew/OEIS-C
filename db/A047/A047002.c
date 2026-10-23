/*
 * A047002  T(n,n), array T given by A047000.
 * https://oeis.org/A047002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1: Let f(n,k) be the number of meanders of n steps
 *            that end at altitude k. Base: f(0,0)=1, f(0,k)=0 for k>0.
 *            Transition: f(n,k)=f(n-1,k-1)+f(n-1,k-2)+f(n-1,k+1)+f(n-1,k+2)
 *            (terms with negative indices or indices >2(n-1) are omitted).
 *            Then a(n) = Σ_{k=0}^{2(n‑1)} f(n‑1,k). Thus a(n)...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *prev = NULL;
    static seq_t *curr = NULL;
    static seq_t *sums = NULL;
    static idx_t max_computed = 0;       /* largest n for which sums[ ] is known */
    static seq_t last_row_sum = 0;       /* sum of row max_computed   */
    static size_t array_cap = 0;         /* capacity of prev/curr     */
    static size_t sums_cap  = 0;         /* capacity of sums array    */

    /* handle trivial case */
    if (n == 0) return 1;

    /* ensure sums array can hold up to n */
    if (n >= sums_cap) {
        size_t new_cap = n + 1;
        sums = seq_realloc(sums, sums_cap, new_cap);
        if (!sums) return (seq_t)0;          /* allocation error */
        /* initialise new elements to 0 */
        for (size_t i = sums_cap; i < new_cap; ++i) sums[i] = 0;
        sums_cap = new_cap;
        /* initialise base if this is the very first call */
        if (max_computed == 0) {
            sums[0] = 1;
        }
    }

    /* if we already have the answer, return it */
    if (n <= max_computed) return sums[n];

    /* compute required row capacity */
    size_t needed_cap = 2 * n + 3;           /* 2*i + 3 for i == n */
    if (array_cap < needed_cap) {
        size_t new_cap = needed_cap;
        prev = seq_realloc(prev, array_cap, new_cap);
        curr = seq_realloc(curr, array_cap, new_cap);
        if (!prev || !curr) return (seq_t)0; /* allocation error */
        /* initialise new cells to 0 */
        for (size_t i = array_cap; i < new_cap; ++i) {
            prev[i] = 0;
            curr[i] = 0;
        }
        array_cap = new_cap;
    }

    /* initialise first row if necessary */
    if (max_computed == 0) {
        for (size_t i = 0; i < array_cap; ++i) prev[i] = 0;
        prev[0] = 1;               /* f(0,0)=1 */
        last_row_sum = 1;           /* sum of row 0 */
        max_computed = 0;
    }

    /* accumulate rows up to n */
    for (idx_t step = max_computed + 1; step <= n; ++step) {
        sums[step] = last_row_sum;           /* a(step) = sum of previous row */

        idx_t max_j   = 2 * step;
        idx_t prev_lim = 2 * (step - 1);

        seq_t row_sum = 0;
        for (idx_t j = 0; j <= max_j; ++j) {
            seq_t val = 0;
            if (j >= 1)           val += prev[(size_t)(j - 1)];
            if (j >= 2)           val += prev[(size_t)(j - 2)];
            if (j + 1 <= prev_lim) val += prev[(size_t)(j + 1)];
            if (j + 2 <= prev_lim) val += prev[(size_t)(j + 2)];
            curr[(size_t)j] = val;
            row_sum += val;
        }

        /* prepare for next step */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
        last_row_sum = row_sum;
    }

    max_computed = n;
    return sums[n];
}
