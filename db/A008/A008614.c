/*
 * A008614  Molien series of 3-dimensional representation of group GL(3,2) (=
 *          L(2,7)); a simple group of order 168.
 * https://oeis.org/A008614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) defined by recurrence: a(0)=1, a(1)=0, a(2)=0, a(3)=0,
 *            a(4)=1, a(5)=0, a(6)=1, a(7)=0, a(8)=1, a(9)=0, a(10)=1,
 *            a(11)=0, a(12)=2, a(13)=0, a(14)=2, a(15)=0, a(16)=2, a(17)=0,
 *            a(18)=3, a(19)=0, a(20)=3, a(21)=1, a(22)=3, a(23)=0; For n >=
 *            24: a(n) = a(n-4) + a(n-6) - a(n-10) + a(n-14) - a(n-1...
 * Range:     u8 n <= 281, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max = (idx_t)-1;

static const seq_t base_vals[24] = {
    1ULL, 0ULL, 0ULL, 0ULL, /* 0..3 */
    1ULL, 0ULL, 1ULL, 0ULL, /* 4..7 */
    1ULL, 0ULL, 1ULL, 0ULL, /* 8..11 */
    2ULL, 0ULL, 2ULL, 0ULL, /* 12..15 */
    2ULL, 0ULL, 3ULL, 0ULL, /* 16..19 */
    3ULL, 1ULL, 3ULL, 0ULL  /* 20..23 */
};

static void ensure_cache(idx_t n)
{
    /* First time: allocate space and fill base values up to 23 */
    if (cache == NULL) {
        cache_cap = 25; /* at least up to index 23 */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return; /* allocation failed; caller will handle zero */
        for (size_t i = 0; i < 24; ++i)
            cache[i] = base_vals[i];
        cache_max = 23;
    }

    /* Extend capacity if needed */
    if ((idx_t)cache_cap <= n) {
        size_t need = (size_t)(n + 1);
        size_t new_cap = cache_cap;
        while (new_cap <= need) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) {
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    /* Compute new entries using recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 23) continue; /* base values already set */
        cache[(size_t)i] =
            cache[(size_t)(i-4)] + cache[(size_t)(i-6)] -
            cache[(size_t)(i-10)] + cache[(size_t)(i-14)] -
            cache[(size_t)(i-18)] - cache[(size_t)(i-20)] +
            cache[(size_t)(i-24)];
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)-1) return 0;            /* guard against overflow */
    ensure_cache(n);
    return cache[(size_t)n];
}
