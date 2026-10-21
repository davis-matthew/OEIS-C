/*
 * A205330  Number of (n+1)X4 0..2 arrays with the number of equal 2X2
 *          subblock diagonal pairs and equal antidiagonal pairs differing
 *          from each horizontal or vertical neighbor, and new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A205330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A205330: Number of (n+1)×4 0..2 arrays with the number of equal 2×2 subblock
 * diagonal pairs and equal antidiagonal pairs differing from each horizontal
 * or vertical neighbor, and new values 0..2 introduced in row major order.
 *
 * Empirical recurrence (OEIS formula):
 *   a(n) = 192*a(n-2)
 *         -3844*a(n-4)
 *         +2656*a(n-5)
 *         +20720*a(n-6)
 *         -30528*a(n-7)
 *         +42944*a(n-8)
 *         -17408*a(n-9)
 *         -103424*a(n-10)
 *         +77824*a(n-11)
 *         -114688*a(n-12)
 *         +65536*a(n-13)
 *
 * Initial values (for n = 1..13):
 *   396, 4860, 63270, 827904, 10839696, 141909312, 1857980448,
 *   24321964032, 318441384576, 4168528605696, 54577498150656,
 *   714442081600512, 9354000126167040
 */

#include <stdlib.h>

/* The following types are supplied by the build environment */
extern seq_t *alloc_cache(idx_t n); /* unused, keeps compiler from warning */
static const seq_t init_vals[13] = {
    (seq_t)396ULL,
    (seq_t)4860ULL,
    (seq_t)63270ULL,
    (seq_t)827904ULL,
    (seq_t)10839696ULL,
    (seq_t)141909312ULL,
    (seq_t)1857980448ULL,
    (seq_t)24321964032ULL,
    (seq_t)318441384576ULL,
    (seq_t)4168528605696ULL,
    (seq_t)54577498150656ULL,
    (seq_t)714442081600512ULL,
    (seq_t)9354000126167040ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* safety for out‑of‐range */

    static seq_t *cache = NULL;            /* cached values (index 0 unused) */
    static idx_t cache_size = 0;           /* largest n for which cache[i] is valid */
    static idx_t cache_cap = 0;            /* allocated capacity (in elements) */

    /* initialise cache with the first 13 terms */
    if (!cache) {
        cache_cap = 14;                     /* need indices 0..13, 0 unused */
        cache = (seq_t *)calloc((size_t)cache_cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failure (should not happen) */
        for (idx_t i = 1; i <= 13; ++i)
            cache[i] = init_vals[i - 1];
        cache_size = 13;
    }

    /* if n is already cached, just return it */
    if (n <= cache_size)
        return cache[n];

    /* grow the cache if necessary */
    if (n + 1 > cache_cap) {
        cache_cap = n + 1;
        seq_t *new_cache =
            (seq_t *)realloc(cache, (size_t)cache_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;    /* allocation failure */
        cache = new_cache;
    }

    /* compute all missing terms from cache_size+1 up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t pos = 0;
        pos += (seq_t)192ULL  * cache[i - 2];
        pos += (seq_t)2656ULL  * cache[i - 5];
        pos += (seq_t)20720ULL * cache[i - 6];
        pos += (seq_t)42944ULL * cache[i - 8];
        pos += (seq_t)77824ULL * cache[i - 11];
        pos += (seq_t)65536ULL * cache[i - 13];

        seq_t neg = 0;
        neg += (seq_t)3844ULL  * cache[i - 4];
        neg += (seq_t)30528ULL * cache[i - 7];
        neg += (seq_t)17408ULL * cache[i - 9];
        neg += (seq_t)103424ULL * cache[i - 10];
        neg += (seq_t)114688ULL * cache[i - 12];

        cache[i] = pos - neg;              /* unsigned subtraction: wrap‑around not a problem */
    }

    cache_size = n;
    return cache[n];
}
