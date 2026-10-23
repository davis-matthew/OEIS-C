/*
 * A154958  Antidiagonal sums of number triangle A154957 regarded as a lower
 *          triangular infinite matrix.
 * https://oeis.org/A154958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - 2*a(n-3) + a(n-4) + a(n-5) - a(n-6)
 *            with a(0)=1, a(1)=1, a(2)=2, a(3)=1, a(4)=2, a(5)=1
 * Range:     u8 n <= 763, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;          /* allocated size of cache */
static idx_t  computed_max = (idx_t)-1;/* highest index computed */

/* Ensure the cache can store up to index n and compute missing values. */
static void ensure_cache(idx_t n)
{
    /* grow the storage if necessary */
    if (!cache) {
        size_t cap = 8;
        while (cap <= (size_t)n + 1) cap <<= 1;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache_cap = cap;
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first six terms only once */
    if (computed_max == (idx_t)-1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)1;
        computed_max = 5;
    }

    if (computed_max >= n) return;      /* nothing left to compute */

    /* compute remaining terms using the recurrence */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        long long v =
            (long long)cache[i-1] + (long long)cache[i-2]
            - 2LL * (long long)cache[i-3]
            + (long long)cache[i-4]
            + (long long)cache[i-5]
            - (long long)cache[i-6];
        cache[i] = (seq_t)v;              /* cast will truncate to seq_t width */
    }
    computed_max = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
