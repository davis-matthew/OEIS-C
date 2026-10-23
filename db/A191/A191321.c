/*
 * A191321  Number of dispersed Dyck paths of length n (i.e., Motzkin paths of
 *          length n with no (1,0) steps at positive heights) having only
 *          ascents of even length (an ascent is a maximal sequence of
 *          consecutive (1,1)-steps).
 * https://oeis.org/A191321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   This function computes A191321 – the number of dispersed Dyck
 *            paths of length n with only even‑length ascents. The value is
 *            calculated by a dynamic programming algorithm that keeps track
 *            of the current height (0 ≤ h ≤ n) and the parity of the ongoing
 *            ascent (0 = even or not in an ascent, 1 = odd...
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 52, u64 n <= 100,
 *            u128 n <= 194, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* maximum height that can ever be reached is n (if all steps are U) */
    idx_t max_h = n + 1;                      /* allocate a bit more for safety */
    size_t array_len = (size_t)(max_h * 2);   /* 2 parity states per height */

    /* allocate DP arrays */
    seq_t *cur = (seq_t *)calloc(array_len, sizeof(seq_t));
    seq_t *nxt = (seq_t *)calloc(array_len, sizeof(seq_t));
    if (!cur || !nxt) {
        /* allocation failure – return 0 as a safe default */
        return (seq_t)0;
    }

    /* initial state: height 0, even parity (not in an ascent) */
    cur[0] = (seq_t)1;   /* (0*2 + 0) = 0 */

    /* iterate over all n steps */
    for (idx_t step = 0; step < n; ++step) {
        /* zero next array */
        for (size_t i = 0; i < array_len; ++i) nxt[i] = (seq_t)0;

        /* reachable heights at this step are 0 … step */
        idx_t reachable = step;
        for (idx_t h = 0; h <= reachable; ++h) {
            /* parity 0: not in an ascent or just finished one */
            seq_t val0 = cur[(size_t)(h * 2) + 0];
            if (val0 != (seq_t)0) {
                /* place U -> parity becomes 1 */
                nxt[(size_t)((h + 1) * 2) + 1] += val0;
                /* place H only if at height 0 */
                if (h == 0) {
                    nxt[(size_t)(0 * 2) + 0] += val0;
                }
                /* place D only if above height 0 */
                if (h > 0) {
                    nxt[(size_t)((h - 1) * 2) + 0] += val0;
                }
            }

            /* parity 1: already in an odd ascent, can only extend it with U */
            seq_t val1 = cur[(size_t)(h * 2) + 1];
            if (val1 != (seq_t)0) {
                nxt[(size_t)((h + 1) * 2) + 0] += val1;
            }
        }

        /* swap cur and nxt for next iteration */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* answer: height 0, parity even */
    seq_t result = cur[0];  /* (0*2 + 0) */

    /* free temporary arrays */
    free(cur);
    free(nxt);

    return result;
}
