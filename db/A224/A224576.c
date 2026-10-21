/*
 * A224576  Number of (n+5)X12 0..1 matrices with each 6X6 subblock
 *          idempotent.
 * https://oeis.org/A224576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) -3*a(n-2) + a(n-3) + 2*a(n-6) -5*a(n-7)
 *            +4*a(n-8) * - a(n-9) - a(n-12) +2*a(n-13) - a(n-14) for n>18 *
 *            Initial terms a(1)…a(18) are listed below
 * Range:     u8 none, u16 n <= 15, u32 n <= 60, u64 n <= 148, u128 n <= 192,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation (1‑based index) */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* largest n for which cache[n] is valid */
static size_t cache_cap  = 0;   /* current allocation size */

/* first 18 terms, a(1)…a(18) */
static const seq_t init_vals[18] = {
    28708, 7843, 8294, 9164, 9946, 10561, 11032, 12921,
    16072, 20202, 25024, 30332, 35979, 43392, 53814,
    68241, 87358, 111648
};

/* enlarge or allocate the cache if required */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;

    /* allocate or grow the cache storage */
    if (cache == NULL) {
        cache_cap = n > 36 ? n * 2 : 36;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;
    } else if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n) new_cap = n * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first 18 entries up to n (if any are missing) */
    size_t min18 = n < 18 ? (size_t)n : 18;
    for (size_t i = 1; i <= min18; ++i)
        cache[i] = init_vals[i-1];

    /* update current known length */
    if (cache_len < min18) cache_len = min18;

    /* compute the missing terms up to n using the recurrence */
    for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
        seq_t val = 0;
        /* recurrence (safe because references are guaranteed to exist) */
        val +=  3 * cache[i - 1];
        val -=  3 * cache[i - 2];
        val +=  1 * cache[i - 3];
        val +=  2 * cache[i - 6];
        val -=  5 * cache[i - 7];
        val +=  4 * cache[i - 8];
        val -=  1 * cache[i - 9];
        val -=  1 * cache[i -12];
        val +=  2 * cache[i -13];
        val -=  1 * cache[i -14];
        cache[i] = val;
    }
    cache_len = (size_t)n;
}

/* public sequence routine */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* a(0) is outside the defined range */
    ensure_cache(n);
    return cache[n];
}
