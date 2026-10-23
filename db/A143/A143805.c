/*
 * A143805  Eigensequence of triangle A130534.
 * https://oeis.org/A143805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0}^{n-1} C(n,k+1)*a(k) * where C(n,k) are unsigned
 *            Stirling numbers of the first kind. * Their recurrence: C(n,k) =
 *            C(n-1,k-1) + (n-1)*C(n-1,k). * The function memoizes computed
 *            a(n) and the last Stirling row so * that arbitrary call order is
 *            supported
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static storage for memoization */
    static idx_t computed_max = (idx_t)(-1);   /* highest n computed so far */
    static seq_t *a_vals      = NULL;         /* sequence values a(0..computed_max) */
    static size_t a_cap       = 0;            /* allocated capacity of a_vals */
    static seq_t *prev_row    = NULL;         /* Stirling row for the current n   */
    static size_t prev_len    = 0;            /* its length   */

    /* Initialize for n = 0 on first call */
    if (computed_max == (idx_t)(-1)) {
        a_cap = 4;
        a_vals = (seq_t*)malloc(a_cap * sizeof(seq_t));
        a_vals[0] = 1;           /* a(0) = 1 */
        computed_max = 0;

        /* Row for n = 0: C(0,0)=1 */
        prev_len = 1;
        prev_row = (seq_t*)malloc(sizeof(seq_t));
        prev_row[0] = 1;
    }

    /* Return cached value if already known */
    if (n <= computed_max) {
        return a_vals[n];
    }

    /* Extend the sequence up to n */
    for (idx_t m = computed_max + 1; m <= n; ++m) {
        /* Allocate current Stirling row of length m+1 */
        seq_t *curr = (seq_t*)malloc((m + 1) * sizeof(seq_t));
        curr[0] = 0; /* C(m,0) = 0 for m >= 1 */

        /* Build the row using the recurrence */
        for (idx_t k = 1; k <= m; ++k) {
            seq_t part1 = 0, part2 = 0;
            if (k - 1 < prev_len) part1 = prev_row[k - 1];
            if (k < prev_len) {
                part2 = prev_row[k] * (seq_t)(m - 1);
            }
            curr[k] = part1 + part2;
        }

        /* Compute a(m) = sum_{k=0}^{m-1} C(m,k+1)*a(k) */
        seq_t sum = 0;
        for (idx_t k = 0; k <= m - 1; ++k) {
            sum += curr[k + 1] * a_vals[k];
        }

        /* Store the new sequence value, resizing if needed */
        if (a_cap <= m) {
            size_t new_cap = (a_cap == 0 ? 4 : a_cap * 2);
            while (new_cap <= m) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(a_vals, new_cap * sizeof(seq_t));
            a_vals = tmp;
            a_cap = new_cap;
        }
        a_vals[m] = sum;

        /* Advance the Stirling row */
        free(prev_row);
        prev_row = curr;
        prev_len = m + 1;
    }

    computed_max = n;
    return a_vals[n];
}
