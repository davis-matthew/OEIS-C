/*
 * A135318  The Kentucky-2 sequence: a(n) = a(n-2) + 2*a(n-4), with a[0..3] =
 *          [1, 1, 1, 2].
 * https://oeis.org/A135318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-4), a(0)=1, a(1)=1, a(2)=1, a(3)=2
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 64, u64 n <= 128,
 *            u128 n <= 256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined in the build environment */
/* The following function is thread‐safe only if called from a single thread. */
seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for i < cache_len */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Handle the first four values directly */
    if (n < 4) {
        switch (n) {
            case 0:
            case 1:
            case 2: return (seq_t)1;
            case 3: return (seq_t)2;
        }
    }

    /* Initialise the cache if it is currently empty */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) {
            /* In a real program we might abort; here we simply return 0. */
            return (seq_t)0;
        }
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
    }

    /* Grow the cache if needed */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed; fall back to computing only this term */
            /* compute a(n) on the fly without storing anything beyond */
            seq_t v0 = (seq_t)1, v1 = (seq_t)1, v2 = (seq_t)1, v3 = (seq_t)2;
            /* we already know a(0..3), now compute up to n using a sliding window */
            if (n == 0) return v0; /* already handled above, but for completeness */
            if (n == 1) return v1;
            if (n == 2) return v2;
            if (n == 3) return v3;
            idx_t i;
            seq_t ai2 = v2, ai4 = (seq_t)1; /* a(2), a(0) */
            for (i = 4; i <= n; ++i) {
                seq_t ai = ai2 + ((seq_t)2 * ai4);
                ai4 = ai2;    /* shift window: a(i-4) becomes previous a(i-2) */
                ai2 = ai;     /* a(i-2) = a(i) */
            }
            return ai2; /* Value of a(n) is in ai2 after loop */
        }
        cache = tmp;
        /* Compute the newly added terms.  start is the first index that is
           not yet computed.  All indices <4 are already initialised. */
        idx_t start = cache_len;
        if (start < 4) start = 4;
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = cache[i - 2] + ((seq_t)2 * cache[i - 4]);
        }
        cache_len = new_len;
    }

    return cache[n];
}
