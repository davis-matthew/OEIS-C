/*
 * A092264  a(n)*a(n-5) = a(n-1)*a(n-4)+a(n-2)+a(n-3), with initial terms a(1)
 *          = ... = a(5) = 1.
 * https://oeis.org/A092264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 32, u64 n <= 59, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A092264
   Recurrence:
        a(n)*a(n-5) = a(n-1)*a(n-4) + a(n-2) + a(n-3)
   Initial values:  a(1)=a(2)=a(3)=a(4)=a(5)=1
   For n ≥ 13 the linear form can be used:
        a(n) = 28*a(n‑4) – 28*a(n‑8) + a(n‑12)          */

#include <stdlib.h>   /* for malloc and realloc */

static seq_t *cache = NULL;   /* term cache, 1‑based indexing */
static idx_t  cache_len = 0;  /* highest index stored in cache */

/* Compute A092264(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    /* initialise cache and first 12 terms on first call */
    if (cache == NULL) {
        cache_len = 12;
        cache = (seq_t *)malloc((cache_len + 1) * sizeof(seq_t));
        if (!cache) return 0;                 /* allocation failure – unlikely in test */
        cache[0] = 0;                         /* unused element */

        /* a(1)…a(5) = 1 */
        for (idx_t i = 1; i <= 5; ++i) cache[i] = 1;

        /* a(6)…a(12) via the original recurrence */
        for (idx_t i = 6; i <= 12; ++i) {
            seq_t tmp = cache[i-1] * cache[i-4];
            tmp += cache[i-2];
            tmp += cache[i-3];
            cache[i] = tmp / cache[i-5];
        }
    }

    /* return already computed value */
    if (n <= cache_len) return cache[n];

    /* grow the cache to hold indices up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return 0;                 /* allocation failure */
    cache = new_cache;

    /* compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t a_i;
        if (i <= 12) {
            /* use original recurrence for indices 6‑12 (not reached normally) */
            seq_t tmp = cache[i-1] * cache[i-4];
            tmp += cache[i-2];
            tmp += cache[i-3];
            a_i = tmp / cache[i-5];
        } else {
            /* linear recurrence for n ≥ 13 */
            a_i = cache[i-4] * ((seq_t)28);
            a_i -= cache[i-8] * ((seq_t)28);
            a_i += cache[i-12];
        }
        cache[i] = a_i;
    }
    cache_len = n;
    return cache[n];
}
