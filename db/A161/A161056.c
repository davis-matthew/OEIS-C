/*
 * A161056  Number of partitions of n into powers of two where every part
 *          appears at least 7 times.
 * https://oeis.org/A161056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into powers of two, where each
 *            part used appears at least 7 times
 * Range:     u8 n <= 95, u16 n <= 329, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *CACHE = NULL;      /* cached values up to CACHE_SIZE */
static idx_t  CACHE_SIZE = 0;    /* largest n computed and stored */

/* Computes a(n) for n >= 1, using precomputation up to the
   largest argument requested so far.  This memoises the result
   in the static CACHE. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Recompute cache if the current n is beyond the cached range. */
    if (n > CACHE_SIZE) {
        size_t sz = (size_t)(n + 1);

        /* Temporary buffers for the dynamic programming arrays. */
        seq_t *pre = (seq_t*)calloc(sz, sizeof(seq_t));
        seq_t *cur = (seq_t*)calloc(sz, sizeof(seq_t));
        if (!pre || !cur) return 0;   /* Out of memory – fallback */

        /* Base case: only one way to partition 0. */
        pre[0] = 1;

        /* Iterate over all powers of two up to n. */
        for (idx_t s = 1; s <= n; s <<= 1) {
            for (idx_t r = 0; r < s; ++r) {
                seq_t cum = 0;
                for (idx_t k = 0, pos = r; pos <= n; ++k, pos += s) {
                    if (k >= 7)
                        cum += pre[pos - 7 * s];   /* add dp[n‑7s] */
                    cur[pos] = pre[pos] + cum;     /* dp_new[n] = dp_prev[n] + cum */
                }
            }
            /* Prepare for next coin type: swap pre and cur. */
            seq_t *tmp = pre;  pre = cur;  cur = tmp;
        }

        /* After the loop, 'pre' holds the final array. */
        free(cur);                /* cur now holds the old 'pre' before swap */

        /* Replace old cache, if any. */
        if (CACHE) free(CACHE);
        CACHE = pre;               /* keep the final results for future calls */
        CACHE_SIZE = n;
    }

    return CACHE[n];
}
