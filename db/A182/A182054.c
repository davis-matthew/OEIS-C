/*
 * A182054  Number of independent sets of nodes in the generalized Petersen
 *          graph G(2n,2) (n>=0).
 * https://oeis.org/A182054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)+15*a(n-2)+3*a(n-3)-13*a(n-4)+4*a(n-5) Initial
 *            values: a(0)=8, a(1)=3, a(2)=39, a(3)=171, a(4)=1055, a(5)=5828
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Cache for previously computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* capacity of the cache array */
static size_t cache_size = 0;  /* next free index in cache (size of filled part) */

static void ensure_cache(idx_t n)
{
    /* Allocate or enlarge cache as necessary */
    if (cache == NULL) {
        cache_cap = (n < 64 ? 64 : n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache_size = 0;
    }
    while (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= cache_cap) new_cap = cache_cap + 128; /* fallback */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Fill cache up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 6) { /* initial values */
            switch (i) {
            case 0: cache[0] = (seq_t)8;   break;
            case 1: cache[1] = (seq_t)3;   break;
            case 2: cache[2] = (seq_t)39;  break;
            case 3: cache[3] = (seq_t)171; break;
            case 4: cache[4] = (seq_t)1055;break;
            case 5: cache[5] = (seq_t)5828;break;
            }
        } else {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a4 = cache[i-4];
            seq_t a5 = cache[i-5];
            /* Use the recurrence */
            cache[i] = (seq_t)3 * a1 + (seq_t)15 * a2 + (seq_t)3 * a3
                       - (seq_t)13 * a4 + (seq_t)4 * a5;
        }
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
