/*
 * A129167  Number of base pyramids in all skew Dyck paths of semilength n.
 * https://oeis.org/A129167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k*A129165(n,k). * Recurrence: * 2*(n+1)*a(n)
 *            = (13*n-3)*a(n-1) - 4*(4*n-3)*a(n-2) + 5*(n-1)*a(n-3), * initial
 *            values: a(0)=0, a(1)=1, a(2)=3 * * This implementation uses
 *            memoised recurrence computation; it works * for primitive
 *            unsigned integer seq_t types and for an arbitrary...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

#if defined(__SIZEOF_INT128__)
    #define HAVE_INT128 1
#else
    #define HAVE_INT128 0
#endif

#ifdef __cplusplus
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
static idx_t  cache_cap = 0;
#endif
static idx_t cached_upto = 0;

/* Ensure the cache holds at least (n+1) entries */
static void ensure_capacity(idx_t n)
{
#ifdef __cplusplus
    if (cache.size() < n + 1)
        cache.resize(n + 1);
#else
    if (cache_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            exit(1);   /* Out of memory – test harness will not trigger this */
        cache = new_cache;
        cache_cap = (idx_t)new_cap;
    }
#endif
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Handle the small base cases directly */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 3;

    /* Prepare storage for the requested value */
    ensure_capacity(n);

#ifdef __cplusplus
    /* Initialise the base values on first use */
    if (cached_upto < 2) {
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 3;
        cached_upto = 2;
    }
#else
    if (cached_upto < 2) {
        if (cache_cap < 3) {
            size_t new_cap = 3;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache)
                exit(1);
            cache = new_cache;
            cache_cap = (idx_t)new_cap;
        }
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 3;
        cached_upto = 2;
    }
#endif

    /* Compute any missing terms up to n */
    for (idx_t i = cached_upto + 1; i <= n; ++i) {
#if HAVE_INT128
        /* Use 128‑bit arithmetic for small integer seq_t types */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 num = (unsigned __int128)cache[i-1] *
                                   ((unsigned __int128)13 * i - 3);
            num -= (unsigned __int128)cache[i-2] *
                   ((unsigned __int128)4 * ((unsigned __int128)4 * i - 3));
            num += (unsigned __int128)cache[i-3] *
                   ((unsigned __int128)5 * (i - 1));
            cache[i] = (seq_t)(num / (unsigned __int128)(2 * (i + 1)));
            continue;
        }
#endif
        /* Generic sequence arithmetic (works for arbitrary‑precision types) */
        seq_t t1 = cache[i-1] * (seq_t)(13 * i - 3);
        seq_t t2 = cache[i-2] * (seq_t)4 * (seq_t)(4 * i - 3);
        seq_t t3 = cache[i-3] * (seq_t)5 * (seq_t)(i - 1);

        seq_t num = t1 + t3 - t2;           /* numerator is non‑negative */
        seq_t denom = (seq_t)(2 * (i + 1));
        cache[i] = num / denom;
    }
    cached_upto = n;
    return cache[n];
}
