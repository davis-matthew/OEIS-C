/*
 * A074286  Partial sum of the Kolakoski sequence (A000002) minus n.
 * https://oeis.org/A074286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ k ≤ n : A000002(k) = 2 } (number of 2's in the first n
 *            terms)
 * Range:     u8 n <= 509, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Allocate storage for the Kolakoski sequence up to n terms */
    seq_t *seq = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!seq) return 0;  /* out of memory – return 0 as a fallback */

    /* Initial seed of the Kolakoski sequence */
    seq[0] = 1;
    seq[1] = 2;
    seq[2] = 2;

    seq_t count2 = 0;
    /* Count 2's in the already known part */
    for (idx_t j = 0; j < n && j < 3; ++j)
        if (seq[j] == 2) ++count2;

    /* If n ≤ 3 we are done */
    if (n <= 3) {
        free(seq);
        return count2;
    }

    idx_t len = 3;      /* number of terms prepared in seq */
    idx_t i   = 2;      /* index of the next run length to consume */
    seq_t val = 1;      /* value of the current run (runs alternate 1,2,1,2,…) */

    while (len < n) {
        idx_t run_len = (idx_t)seq[i];
        for (idx_t t = 0; t < run_len && len < n; ++t) {
            seq[len] = val;
            if (val == 2) ++count2;
            ++len;
        }
        val = (val == 1) ? 2 : 1;  /* toggle run value */
        ++i;
    }

    free(seq);
    return count2;
}
