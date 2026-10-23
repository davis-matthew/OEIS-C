/*
 * A038619  Smallest positive number that needs more lines when shown on a
 *          7-segment display (digital clock) than any previous term.
 * https://oeis.org/A038619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038619
   a(1)=1,  a(2)=2,  a(3)=6,  a(4)=8,  a(5)=10,  a(6)=18,  a(7)=20,
   a(8)=28,  a(9)=68, a(10)=88, a(11)=108, a(12)=188, a(13)=200.
   For n ≥ 14: a(n) = 10*a(n-7) + 8.  */
#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑based cache of already computed terms */
static size_t  cached_len = 0;   /* last valid index in cache (≥0) */
static int init_done = 0;        /* flag that cache has been initialised */

/* initialise the first 13 terms of the sequence */
static void init_terms(void)
{
    if (init_done)
        return;
    /* Ensure the cache can hold at least 13 entries */
    size_t cap = cached_len + 1;
    if (cap < 14)
        cap = 14;
    if (cache == NULL) {
        cache = (seq_t *)malloc((cap + 1) * sizeof(seq_t)); /* index 0 unused */
        if (!cache) abort();
        cached_len = 0;
    } else if (cap > cached_len + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, (cap + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }
    static const seq_t init[14] = {
        0ULL, /* unused */
        1ULL, /* a(1) */
        2ULL, /* a(2) */
        6ULL, /* a(3) */
        8ULL, /* a(4) */
        10ULL,/* a(5) */
        18ULL,/* a(6) */
        20ULL,/* a(7) */
        28ULL,/* a(8) */
        68ULL,/* a(9) */
        88ULL,/* a(10) */
        108ULL,/* a(11) */
        188ULL,/* a(12) */
        200ULL /* a(13) */
    };
    for (size_t i = 1; i <= 13; ++i)
        cache[i] = init[i];
    cached_len = 13;
    init_done = 1;
}

/* make sure the cache has room for indices up to n (1‑based) */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n;
    if (need < 13)          /* we always need at least 13 slots for init */
        need = 13;
    if (cache == NULL) {
        cache = (seq_t *)malloc((need + 1) * sizeof(seq_t));
        if (!cache) abort();
        cached_len = 0;
    } else if (need > cached_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (need + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }
}

/* Memoised computation of A038619. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for 0, but return 0 */

    ensure_cache(n);
    init_terms();
    if (n <= cached_len)
        return cache[n];

    for (idx_t i = cached_len + 1; i <= n; ++i) {
        /* For n ≥ 14 use a(n) = 10*a(n-7) + 8 */
        cache[i] = ((seq_t)10 * cache[i - 7]) + (seq_t)8;
    }
    cached_len = n;
    return cache[n];
}
