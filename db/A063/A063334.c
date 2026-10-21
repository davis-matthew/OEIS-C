/*
 * A063334  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          61 ).
 * https://oeis.org/A063334
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-4)+a(n-6)-a(n-10) for n>13, a(2) = -1, a(3) = 126,
 *            a(4) = 280, a(5) = 435, a(6) = 590, a(7) = 745, a(8) = 900, a(9)
 *            = 1055, a(10)=1210, a(11)=1365, a(12)=1520, a(13)=1673
 * Range:     i8 n <= 3, i16 n <= 213, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#if defined(__cplusplus)
#include <vector>
#endif

/* Cache for already computed terms */
static idx_t cache_limit   = 0;   /* storage size-1 (>= cached index) */
static idx_t computed_up   = 0;   /* largest n for which cache[·] is defined */

#if defined(__cplusplus)
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
#endif

/* initialise base values 2..13 */
static void init_base(void)
{
#if defined(__cplusplus)
    if (cache.size() < 14)
        cache.resize(14);
    cache[2]  = (seq_t)-1;
    cache[3]  = (seq_t)126;
    cache[4]  = (seq_t)280;
    cache[5]  = (seq_t)435;
    cache[6]  = (seq_t)590;
    cache[7]  = (seq_t)745;
    cache[8]  = (seq_t)900;
    cache[9]  = (seq_t)1055;
    cache[10] = (seq_t)1210;
    cache[11] = (seq_t)1365;
    cache[12] = (seq_t)1520;
    cache[13] = (seq_t)1673;
    computed_up = 13;
#else
    if (cache_limit < 13) {
        if (cache) free(cache);
        cache = (seq_t*)malloc((14)*sizeof(seq_t));
        cache_limit = 13;
    }
    cache[2]  = (seq_t)-1;
    cache[3]  = (seq_t)126;
    cache[4]  = (seq_t)280;
    cache[5]  = (seq_t)435;
    cache[6]  = (seq_t)590;
    cache[7]  = (seq_t)745;
    cache[8]  = (seq_t)900;
    cache[9]  = (seq_t)1055;
    cache[10] = (seq_t)1210;
    cache[11] = (seq_t)1365;
    cache[12] = (seq_t)1520;
    cache[13] = (seq_t)1673;
    computed_up = 13;
#endif
}

/* grow the cache so that it can store up to index n */
static void grow_cache(idx_t n)
{
    if (n <= cache_limit) return;
#if defined(__cplusplus)
    cache.resize(n + 1);
    cache_limit = n;
#else
    seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    cache = tmp;
    cache_limit = n;
#endif
}

/* compute all terms up to n using the recurrence */
static void ensure_upto(idx_t n)
{
    if (n <= computed_up) return;
    if (computed_up < 13) init_base();
    grow_cache(n);

    idx_t start = computed_up + 1;
    if (start < 14) start = 14;
    for (idx_t i = start; i <= n; ++i) {
#if defined(__cplusplus)
        cache[i] = cache[i-4] + cache[i-6] - cache[i-10];
#else
        cache[i] = cache[i-4] + cache[i-6] - cache[i-10];
#endif
    }
    computed_up = n;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* undefined below offset, return 0 */
    ensure_upto(n);
    return cache[n];
}
