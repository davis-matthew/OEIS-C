/*
 * A160990  Number of partitions of n where every part appears at least 20
 *          times.
 * https://oeis.org/A160990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 153, u16 n <= 333, u32 n <= 866, u64 n <= 2603,
 *            u128 n <= 8701, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A160990: Number of partitions of n where every part appears at least 20 times.
   Computed via dynamic programming. For each part size s, we add partitions
   where s appears at least 20 times, using the recurrence
        dp_s[n] = dp_prev[n - 20*s] + dp_s[n - s]   (n ≥ 20*s)
   and then merge dp_s into dp_prev. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == (idx_t)0)
        return (seq_t)1;

    size_t N = (size_t)n;
    seq_t *dp_prev = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *dp_s    = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (dp_prev == NULL || dp_s == NULL) {
        free(dp_prev);
        free(dp_s);
        return (seq_t)0;                     /* allocation failure */
    }

    dp_prev[0] = (seq_t)1;                    /* empty partition */

    const idx_t m = 20;                       /* minimal multiplicity */

    for (idx_t s = 1; s <= n; ++s) {
        idx_t start = m * s;                  /* earliest index where s is usable */
        if (start > n) break;                /* further s cannot contribute */

        /* Clear dp_s for indices that will be written */
        for (size_t i = 0; i <= N; ++i) dp_s[i] = (seq_t)0;

        /* Compute dp_s[n] for n ≥ start using the recurrence */
        for (idx_t i = start; i <= n; ++i) {
            seq_t val = dp_prev[i - m * s];
            if (i >= s) val += dp_s[i - s];
            dp_s[(size_t)i] = val;
        }

        /* Merge contributions of part size s into dp_prev */
        for (size_t i = 0; i <= N; ++i) {
            dp_prev[i] += dp_s[i];
        }
    }

    seq_t result = dp_prev[(size_t)n];
    free(dp_prev);
    free(dp_s);
    return result;
}
