/*
 * A026125  a(n) = number of (s(0),s(1),...,s(n)) such that every s(i) is a
 *          nonnegative integer, s(0) = 1, s(n) = 4, |s(1) - s(0)| = 1, |s(i)
 *          - s(i-1)| <= 1 for i >= 2. Also a(n) = T(n,n-3), where T is the
 *          array in A026120.
 * https://oeis.org/A026125
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of walks from 1 to 4 in n steps on ℕ with steps ∈
 *            {−1, 0, 1}, first step forced to be ±1. Computed by dynamic
 *            programming in Θ(n²) time
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 3) /* offset 3 – values below are undefined */
        return (seq_t)0;

    /* Arrays large enough to hold all positions that can occur
       during n steps (max position is 1 + n). */
    size_t sz = (size_t)n + 3;
    seq_t *prev = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!prev || !cur)
        return (seq_t)0;                 /* allocation failed, unlikely */

    prev[1] = (seq_t)1;                   /* start at position 1 */

    for (idx_t step = 1; step <= n; ++step) {
        idx_t max_prev = 1 + (step - 1);   /* highest position reachable
                                             after step-1 */
        idx_t max_cur  = 1 + step;         /* highest position after this step */

        for (idx_t x = 0; x <= max_cur; ++x) {
            seq_t val = 0;
            if (x <= max_prev)               val += prev[x];
            if (x > 0 && x - 1 <= max_prev)  val += prev[x - 1];
            if (x + 1 <= max_prev)           val += prev[x + 1];
            cur[x] = val;
        }

        /* First step must be of size 1, so forbid staying at position 1. */
        if (step == 1)
            cur[1] = (seq_t)0;

        /* swap buffers for next iteration */
        seq_t *tmp = prev; prev = cur; cur = tmp;
    }

    seq_t result = prev[4];                 /* value at position 4 after n steps */

    free(prev);
    free(cur);

    return result;
}
