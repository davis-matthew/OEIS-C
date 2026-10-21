/*
 * A211529  Number of -1..1 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having two,
 *          four, five or six distinct values for every i,j,k<=n.
 * https://oeis.org/A211529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 9*a(n-3) + 10*a(n-4) + 2*a(n-5) -
 *            4*a(n-6) Initial values: a(1)=8, a(2)=22, a(3)=42, a(4)=80,
 *            a(5)=140, a(6)=254
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache      = NULL; /* array of computed values, index 0 unused */
static size_t cache_cap  = 0;    /* allocated capacity (number of elements) */
static size_t cache_used = 0;    /* highest index that has a valid value */

/* stored initial values for indices 0..6 (0 is unused) */
static const seq_t init_vals[7] = {0, 8, 22, 42, 80, 140, 254};

/* ensure the helper can hold indices up to n */
static void grow_cache(idx_t n)
{
    size_t needed = (size_t)n + 1; /* indices 0..n inclusive */
    if (needed <= cache_cap) return;
    seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
    if (!tmp) abort();            /* out‑of‑memory abort */
    cache      = tmp;
    cache_cap  = needed;
}

/* public function: returns a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for 0 but safe fallback */

    /* initialise cache on first invocation */
    if (!cache) {
        cache = (seq_t*)malloc(7 * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 7; ++i)
            cache[i] = init_vals[i];
        cache_cap  = 7;          /* indices 0..6 allocated */
        cache_used = 6;          /* values up to 6 are valid */
    }

    grow_cache(n);

    if ((size_t)n <= cache_used)
        return cache[(size_t)n]; /* already computed */

    /* compute all missing values from cache_used+1 up to n */
    for (idx_t i = (idx_t)(cache_used + 1); i <= n; ++i) {
        seq_t val;
        /* use a 128‑bit intermediate for small types to avoid overflow */
        if (sizeof(seq_t) <= 8) {
            signed __int128 v =
                (signed __int128)4  * (signed __int128)cache[(size_t)i-1] -
                (signed __int128)2  * (signed __int128)cache[(size_t)i-2] -
                (signed __int128)9  * (signed __int128)cache[(size_t)i-3] +
                (signed __int128)10 * (signed __int128)cache[(size_t)i-4] +
                (signed __int128)2  * (signed __int128)cache[(size_t)i-5] -
                (signed __int128)4  * (signed __int128)cache[(size_t)i-6];
            val = (seq_t)v; /* truncate to seq_t size */
        } else {
            val = (seq_t)4  * cache[(size_t)i-1]
                - (seq_t)2  * cache[(size_t)i-2]
                - (seq_t)9  * cache[(size_t)i-3]
                + (seq_t)10 * cache[(size_t)i-4]
                + (seq_t)2  * cache[(size_t)i-5]
                - (seq_t)4  * cache[(size_t)i-6];
        }
        cache[(size_t)i] = val;
    }

    cache_used = (size_t)n;
    return cache[(size_t)n];
}
