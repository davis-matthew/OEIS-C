/*
 * A237012  Number of (n+1)X(5+1) 0..3 arrays with the sum of all four
 *          elements of every 2X2 subblock equal.
 * https://oeis.org/A237012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A237012 – number of (n+1)×6 0..3 arrays with all 2×2 subblocks having the same sum.
    Recurrence (valid for n ≥ 16):

        a(n) = 8 a(n-1) + 16 a(n-2) − 268 a(n-3) + 189 a(n-4)
               + 3388 a(n-5) − 5824 a(n-6) − 20048 a(n-7)
               + 49756 a(n-8) + 52752 a(n-9) − 196896 a(n-10)
               − 24192 a(n-11) + 369216 a(n-12) − 131328 a(n-13)
               − 262656 a(n-14) + 165888 a(n-15)

    Base values (a(1) … a(15)) are taken from the OEIS entry. */

#include <stddef.h>
#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;      /* 1‑based array, cache[1] = a(1) */
static idx_t cache_limit = 0;    /* highest n whose a(n) is stored */

/* base values a(1)..a(15) */
static const seq_t base_vals[15] = {
    18496ull, 30248ull, 53200ull, 108824ull, 235456ull,
    566936ull, 1419088ull, 3823352ull, 10606336ull, 31051928ull,
    93101200ull, 291198584ull, 929945536ull, 3075627416ull,
    10363357648ull
};

/* initialise cache large enough to contain all known base values */
static void init_cache(idx_t request_n)
{
    size_t size_needed = request_n > 15 ? (size_t)request_n : 15;
    cache = (seq_t *)calloc(size_needed + 1, sizeof(seq_t));
    for (idx_t i = 1; i <= 15; ++i)
        cache[i] = base_vals[i - 1];
    cache_limit = 15;   /* we now have a(1)..a(15) */
}

/* ensure that cache contains all terms up to n (including n) */
static void ensure_cache(idx_t n)
{
    if (!cache) {
        init_cache(n);
        if (n <= 15) return;   /* all requested values already available */
    }

    if (n <= cache_limit) return;   /* nothing to compute */

    /* enlarge cache array if necessary */
    if ((size_t)n + 1 > (size_t)cache_limit + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        if (!tmp) return;        /* out‑of‑memory – silently abort */
        cache = tmp;
    }

    /* compute missing terms using the recurrence */
    for (idx_t k = cache_limit + 1; k <= n; ++k) {
        seq_t val = 8   * cache[k - 1] + 16   * cache[k - 2]
                  + 189 * cache[k - 4] + 3388 * cache[k - 5]
                  + 49756 * cache[k - 8] + 52752 * cache[k - 9]
                  + 369216 * cache[k - 12] + 165888 * cache[k - 15];

        val -= 268  * cache[k - 3] + 5824  * cache[k - 6]
              + 20048 * cache[k - 7] + 196896 * cache[k - 10]
              + 24192 * cache[k - 11] + 131328* cache[k - 13]
              + 262656 * cache[k - 14];
        cache[k] = val;
    }
    cache_limit = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for 0; return 0 as a safe value */
    ensure_cache(n);
    return cache[n];
}
