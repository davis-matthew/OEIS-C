/*
 * A154655  Number of permutations of length n within distance 6.
 * https://oeis.org/A154655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Number of permutations of length n with displacement ≤6,
 *            computed by dynamic programming on a 13‑band matrix using a
 *            13‑bit sliding window
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* K = maximal displacement (6) */
static const int K = 6;
static const int W = 2 * K + 1;                 /* window width = 13          */
static const unsigned int FULL_MASK = (1u << W) - 1; /* all bits set */

/* DP for a(n) */
seq_t
a(idx_t n)
{
    if (n == 0)                /* a(0) = 1 */
        return 1;

    /* allocate table of out–of–range masks (length n+2, 1‑based) */
    unsigned int *outMask = (unsigned int *)malloc((size_t)(n + 2) * sizeof(unsigned int));
    if (!outMask) return 0; /* (should never happen) */

    /* precompute out-of-range masks for positions 1 … n+1 */
    for (idx_t i = 1; i <= n + 1; ++i) {
        unsigned int m = 0;
        for (int p = 0; p < W; ++p) {
            int j = (int)(i - K + p);
            if (j < 1 || (idx_t)j > n) /* value outside [1,n] */
                m |= 1u << p;
        }
        outMask[i] = m;
    }

    /* allocate DP arrays */
    seq_t *cur  = (seq_t *)calloc((size_t)(FULL_MASK + 1), sizeof(seq_t));
    seq_t *nxt  = (seq_t *)calloc((size_t)(FULL_MASK + 1), sizeof(seq_t));
    if (!cur || !nxt) {
        free(cur); free(nxt); free(outMask);
        return 0;
    }

    /* initial mask: values < 1 are considered matched */
    unsigned int startMask = 0;
    for (int p = 0; p < W; ++p) {
        int j = 1 - K + p;            /* j = 1-K+p */
        if (j < 1)
            startMask |= 1u << p;
    }
    cur[startMask] = 1;                 /* one way to start */

    /* DP over positions 1 … n */
    for (idx_t i = 1; i <= n; ++i) {
        /* clear next array */
        memset(nxt, 0, (size_t)(FULL_MASK + 1) * sizeof(seq_t));

        unsigned int outM = outMask[i];
        unsigned int outM_next = outMask[i + 1];

        for (unsigned int mask = 0; mask <= FULL_MASK; ++mask) {
            seq_t ways = cur[mask];
            if (!ways) continue;

            /* mask for transitions (out‑of‑range bits forced to 1) */
            unsigned int tmask = mask | outM;

            /* iterate possible matches for position i */
            for (int p = 0; p < W; ++p) {
                if (tmask & (1u << p)) continue;            /* already used */

                /* leftmost value (j = i-K) must have been used unless we match it here */
                if ((tmask & 1u) == 0 && p != 0) continue;

                unsigned int m2 = tmask | (1u << p);         /* also mark this j as used */
                unsigned int newMask = m2 >> 1;              /* slide window to i+1 */
                unsigned int nextMask = newMask | outM_next;

                nxt[nextMask] += ways;
            }
        }

        /* swap cur and nxt for next iteration */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* final answer: all bits must be matched */
    seq_t result = cur[FULL_MASK];

    free(cur);
    free(nxt);
    free(outMask);
    return result;
}
