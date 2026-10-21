/*
 * A200254  Number of 0..n arrays x(0..4) of 5 elements with each no smaller
 *          than the sum of its previous elements modulo (n+1).
 * https://oeis.org/A200254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of 0..n arrays x[0..4] of length 5 where for i≥1 x[i] ≥
 *            (x[0]+…+x[i-1]) mod (n+1). Recurrence: Let M = n+1, dp_k[s] =
 *            number of length‑k prefixes having partial sum ≡ s (mod M).
 *            Initialise dp_1[s] = 1 (s = 0…M‑1). Transition: dp_{k+1}[t] =
 *            Σ_{s: 2s ≤ t ≤ M-1 or max(0,2s−M) ≤ t ≤ s-1} dp_k[...
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 145, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Minimum index expected is 1; values for n<1 are undefined. */
    if (n < 1)
        return 0;

    /* M = n+1 must fit in size_t for allocation. */
    size_t M = (size_t)n + 1;
    if (M == 0)          /* overflow of n+1 (only if n == UINT64_MAX) */
        return 0;

    seq_t *cur  = (seq_t *)calloc(M, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(M, sizeof(seq_t));
    if (!cur || !next) {
        free(cur); free(next);
        return 0;
    }

    /* initial prefixes: one choice for each possible first element */
    for (size_t s = 0; s < M; ++s)
        cur[s] = 1;

    /* 4 transitions to reach 5 elements */
    for (int step = 0; step < 4; ++step) {
        /* Prefix sums of cur */
        seq_t *pref = (seq_t *)calloc(M, sizeof(seq_t));
        if (!pref) {
            free(cur); free(next);
            return 0;
        }
        seq_t acc = 0;
        for (size_t i = 0; i < M; ++i) {
            acc += cur[i];
            pref[i] = acc;
        }

        /* Compute next state */
        for (size_t t = 0; t < M; ++t) {
            /* part1: sum over s with 2s ≤ t  (i.e. s ≤ floor(t/2)) */
            size_t half = t >> 1;          /* floor(t/2) */
            seq_t part1 = pref[half];

            /* part2: sum over s with max(0,2s-M) ≤ t ≤ s-1 */
            seq_t part2 = 0;
            size_t s_min = t + 1;                  /* s ≥ t+1  */
            size_t s_max = (t + M) >> 1;           /* s ≤ floor((t+M)/2) */
            if (s_min <= s_max && s_min < M) {
                seq_t pref_max   = pref[s_max];
                seq_t pref_before = (s_min == 0) ? 0 : pref[s_min - 1];
                part2 = pref_max - pref_before;
            }
            next[t] = part1 + part2;
        }

        free(pref);

        /* Prepare for next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
        memset(next, 0, M * sizeof(seq_t));
    }

    /* Sum over all final states to get total number of arrays */
    seq_t result = 0;
    for (size_t s = 0; s < M; ++s)
        result += cur[s];

    free(cur);
    free(next);
    return result;
}
