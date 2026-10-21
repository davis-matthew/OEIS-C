/*
 * A211303  a(n) = number of n-lettered words in the alphabet {1, 2, 3, 4}
 *          with as many occurrences of the substring (consecutive subword)
 *          [1, 1] as of [1, 2].
 * https://oeis.org/A211303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of n‑letter words over the alphabet {1,2,3,4}
 * with as many occurrences of the substring [1,1] as of [1,2].
 * The implementation uses dynamic programming on the difference
 * (#11 – #12) which can range from –n to +n.
 */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 0 with offset 0 (a(0)=1) */
    if (n == 0) return (seq_t)1;

    /* array indices correspond to difference d offset by n */
    size_t sz  = (size_t)n * 2 + 1;
    size_t off = n;                      /* index representing difference 0 */

    /* allocate zero‑initialized arrays for current and next state */
    seq_t *S0 = (seq_t *)calloc(sz, sizeof(seq_t)); /* last letter ≠ 1, diff d */
    seq_t *S1 = (seq_t *)calloc(sz, sizeof(seq_t)); /* last letter  = 1, diff d */
    seq_t *T0 = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *T1 = (seq_t *)calloc(sz, sizeof(seq_t));

    if (!S0 || !S1 || !T0 || !T1) {
        /* allocation failed – return 0 (should not happen for the test range) */
        free(S0); free(S1); free(T0); free(T1);
        return (seq_t)0;
    }

    /* word of length 1 */
    S0[off] = 3;  /* choose symbol 2,3,4 */
    S1[off] = 1;  /* choose symbol 1 */

    for (idx_t pos = 1; pos < n; ++pos) {
        /* clear next‑state arrays */
        memset(T0, 0, sz * sizeof(seq_t));
        memset(T1, 0, sz * sizeof(seq_t));

        /* current range of reachable differences |d| ≤ pos-1 */
        size_t range = (size_t)pos - 1;
        size_t i_min = off - range;
        size_t i_max = off + range;

        for (size_t i = i_min; i <= i_max; ++i) {
            seq_t val0 = S0[i];
            if (val0) {
                /* previous last symbol ≠ 1 */
                T1[i]     += val0;       /* append 1 → last becomes 1, d unchanged */
                T0[i]     += val0 * 3;   /* append 2,3,4 → last≠1, d unchanged */
            }
            seq_t val1 = S1[i];
            if (val1) {
                /* previous last symbol = 1 */
                T1[i + 1] += val1;       /* append 1 → d increases by 1 */
                T0[i - 1] += val1;       /* append 2 → d decreases by 1 */
                T0[i]     += val1 * 2;   /* append 3,4 → d unchanged */
            }
        }

        /* swapping current and next states */
        seq_t *tmp;

        tmp = S0; S0 = T0; T0 = tmp;
        tmp = S1; S1 = T1; T1 = tmp;
    }

    /* sum over both possible last symbols with difference 0 */
    seq_t result = S0[off] + S1[off];

    /* free allocated memory */
    free(S0);
    free(S1);
    free(T0);
    free(T1);

    return result;
}
