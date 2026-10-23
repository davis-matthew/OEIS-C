/*
 * A147561  Number of representations of n in the Fibonacci-squared base
 *          system. The columns are ..., 64, 25, 9, 4, 1, 1 = ..., 8^2, 5^2,
 *          3^2, 2^2, 1^2, 1^2, i.e., the Fibonacci numbers A000045 squared.
 *          The 'digits' are 0, 1 or 2.
 * https://oeis.org/A147561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 308, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count representations of n in the Fibonacci‑squared base
   system with digits {0,1,2}.  Computed via a dynamic programming
   that iterates over the Fibonacci squares (1,1,4,9,25,…) and
   allows each weight to be used 0, 1 or 2 times. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t *cache   = NULL;
    static idx_t  cache_size = 0;

    /* Allocate on first call (array[0] unused) */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t)); /* size 1 */
        cache[0] = 0;
        cache_size = 1;
    }

    /* Extend cache if requested n is larger */
    if (n >= cache_size) {
        idx_t maxN = n;
        /* ---- compute all Fibonacci squares <= maxN ---- */
        seq_t f1 = 1, f2 = 1;
        seq_t weights[64];          /* 64 is sufficient for maxN ~ 10⁴ */
        idx_t weight_cnt = 0;
        while (1) {
            seq_t sq = f1 * f1;
            if (sq > (seq_t)maxN) break;
            weights[weight_cnt++] = sq;
            seq_t f_next = f1 + f2;
            f1 = f2;  f2 = f_next;
        }

        /* ---- dynamic programming for coin change ---- */
        size_t sz = (size_t)(maxN + 1);
        seq_t *cur   = (seq_t*)calloc(sz, sizeof(seq_t));
        seq_t *next  = (seq_t*)calloc(sz, sizeof(seq_t));
        cur[0] = 1;                     /* one way to make 0 */

        for (idx_t wi = 0; wi < weight_cnt; ++wi) {
            seq_t w  = weights[wi];
            seq_t w2 = w + w;          /* 2*w */

            for (idx_t s = 0; s <= maxN; ++s) {
                seq_t val = cur[s];    /* d = 0 */
                if (s >= (idx_t)w)   val += cur[s - (idx_t)w];
                if (s >= (idx_t)w2)  val += cur[s - (idx_t)w2];
                next[s] = val;
            }
            /* swap cur and next for next iteration */
            seq_t *tmp = cur; cur = next; next = tmp;
        }

        /* ---- store results in cache ---- */
        cache = (seq_t*)realloc(cache, (size_t)(maxN + 1) * sizeof(seq_t));
        for (idx_t i = cache_size; i <= maxN; ++i) {
            cache[i] = cur[i];
        }
        cache_size = maxN + 1;

        free(cur);
        free(next);
    }

    return cache[n];
}
