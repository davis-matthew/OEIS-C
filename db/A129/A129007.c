/*
 * A129007  a(n) = (n^3+n^2)*7^n.
 * https://oeis.org/A129007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3+n^2)*7^n Recurrence:
 *            a(n)=28*a(n-1)-294*a(n-2)+1372*a(n-3)-2401*a(n-4) Initial terms:
 *            a(1)=14, a(2)=588, a(3)=12348, a(4)=192080
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

static idx_t computed_max = 0;

#ifdef __cplusplus
static std::vector<seq_t> cacheVec;
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
#endif

/* Ensure that the memory for indices up to n is allocated.  It does
   not change computed_max. */
static void ensure_capacity(idx_t n) {
    if (n < 1) return;
    size_t needed = (size_t)n + 1;           /* indices 0..n inclusive */
#ifdef __cplusplus
    if (needed > cacheVec.size())
        cacheVec.resize(needed);
#else
    if (needed > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 8 : cache_cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                    /* simple error handling */
        cache = tmp;
        cache_cap = new_cap;
    }
#endif
}

/* Initialise the first four terms.  After calling this,
   computed_max == 4. */
static void init_base(void) {
    ensure_capacity(4);
#ifdef __cplusplus
    cacheVec[1] = 14;
    cacheVec[2] = 588;
    cacheVec[3] = 12348;
    cacheVec[4] = 192080;
#else
    cache[1] = 14;
    cache[2] = 588;
    cache[3] = 12348;
    cache[4] = 192080;
#endif
    computed_max = 4;
}

/* Compute a(n) for n >= 1 using memoised recurrence. */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* behaviour below offset not used */

    ensure_capacity(n);

    if (computed_max >= n) {
#ifdef __cplusplus
        return cacheVec[n];
#else
        return cache[n];
#endif
    }

    if (computed_max < 4) init_base();

    for (idx_t i = computed_max + 1; i <= n; ++i) {
#ifndef __cplusplus
        seq_t t1 = 28 * cache[i-1];
        seq_t t2 = 1372 * cache[i-3];
        seq_t t3 = 294 * cache[i-2];
        seq_t t4 = 2401 * cache[i-4];
        cache[i] = t1 + t2 - t3 - t4;
#else
        seq_t t1 = 28 * cacheVec[i-1];
        seq_t t2 = 1372 * cacheVec[i-3];
        seq_t t3 = 294 * cacheVec[i-2];
        seq_t t4 = 2401 * cacheVec[i-4];
        cacheVec[i] = t1 + t2 - t3 - t4;
#endif
    }
    computed_max = n;

#ifdef __cplusplus
    return cacheVec[n];
#else
    return cache[n];
#endif
}
