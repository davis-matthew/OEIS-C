/*
 * A078480  Number of permutations p of {1,2,...,n} such that |p(i)-i| != 1
 *          for all i.
 * https://oeis.org/A078480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A078480 – Number of permutations p of {1,2,…,n}
 * such that |p(i)-i| != 1 for all i.
 * Recurrence (n ≥ 5):
 *   a(n) = n*a(n-1) - (n-2)*a(n-3) - a(n-4)
 * Initial values:
 *   a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=5
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamic cache of computed values */
static idx_t cache_len = 0;       /* current allocated size (highest index + 1) */

/* Ensure the cache can hold values up to n, expanding if necessary */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;
    idx_t newlen = n + 1;
    seq_t *newarr;
#if defined(__cplusplus)
    newarr = new seq_t[newlen]();   /* value‑initialise all entries */
#else
    newarr = (seq_t *)malloc(sizeof(seq_t) * newlen);
    for (idx_t i = 0; i < newlen; ++i) newarr[i] = 0;
#endif
    if (cache) {
        for (idx_t i = 0; i < cache_len; ++i) newarr[i] = cache[i];
#if defined(__cplusplus)
        delete[] cache;
#else
        free(cache);
#endif
    }
    cache      = newarr;
    cache_len  = newlen;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (cache[n] != 0) return cache[n];        /* value already computed */

    /* Handle base cases */
    if (n == 0) { cache[0] = 1; return 1; }
    if (n == 1) { cache[1] = 1; return 1; }
    if (n == 2) { cache[2] = 1; return 1; }
    if (n == 3) { cache[3] = 2; return 2; }
    if (n == 4) { cache[4] = 5; return 5; }

    /* Incrementally compute values up to n */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t term1 = (seq_t)i * cache[i - 1];
        seq_t term2 = (seq_t)(i - 2) * cache[i - 3];
        seq_t term3 = cache[i - 4];
        cache[i] = term1 - term2 - term3;   /* recurrence */
    }
    return cache[n];
}
