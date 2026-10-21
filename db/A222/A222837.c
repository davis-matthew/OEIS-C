/*
 * A222837  Number of nX7 0..3 arrays with no element equal to another at a
 *          city block distance of exactly two, and new values 0..3 introduced
 *          in row major order.
 * https://oeis.org/A222837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 20, u64 n <= 48, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence (OEIS A222837):
   a(n) = 2*a(n-1) + a(n-3) - 2*a(n-4) + 4*a(n-5) + a(n-6) - a(n-9)   for n>14
   Initial values (offset = 1):
     a(1)=163, a(2)=6144, a(3)=5832, a(4)=10584, a(5)=22848, a(6)=55296,
     a(7)=138240, a(8)=301056, a(9)=642048, a(10)=1382400, a(11)=3022848,
     a(12)=6690816, a(13)=14816256, a(14)=32741376 */
#include <stdlib.h>

static seq_t *cache       = NULL;      /* array 0..n   (index 0 unused)          */
static size_t cache_size  = 0;         /* allocated slots (>= needed)          */
static size_t cache_len   = 0;         /* highest n whose value is stored      */

/* 14 hard‑coded initial terms */
static const seq_t init[14] = {
    (seq_t)163ULL,
    (seq_t)6144ULL,
    (seq_t)5832ULL,
    (seq_t)10584ULL,
    (seq_t)22848ULL,
    (seq_t)55296ULL,
    (seq_t)138240ULL,
    (seq_t)301056ULL,
    (seq_t)642048ULL,
    (seq_t)1382400ULL,
    (seq_t)3022848ULL,
    (seq_t)6690816ULL,
    (seq_t)14816256ULL,
    (seq_t)32741376ULL
};

/*  Ensure that cache[1..n] is filled, allocating and extending it as necessary */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;                     /* already available */

    size_t old_len = cache_len;       /* save current maximum index */

    /* First allocation – also set the 14 base values */
    if (cache_len == 0) {
        size_t needed = 15;           /* indices 0..14 inclusive              */
        cache = (seq_t*)malloc(needed * sizeof(seq_t));
        if (!cache) abort();          /* out of memory */
        cache_size = needed;
        for (size_t i = 0; i < needed; ++i)
            cache[i] = 0;
        for (size_t i = 0; i < 14; ++i)
            cache[i + 1] = init[i];
        cache_len = 14;              /* we have stored up to a(14)          */
    }

    /* Grow the array if n exceeds current capacity */
    if (cache_size <= (size_t)n) {
        size_t old_size = cache_size;
        size_t new_size = (size_t)n + 1;   /* indices 0..n inclusive */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (size_t i = old_size; i <= (size_t)n; ++i)
            cache[i] = 0;
        cache_size = new_size;
    }

    /* Compute any missing terms strictly after old_len */
    for (size_t i = old_len + 1; i <= (size_t)n; ++i) {
        seq_t val;
        if (i <= 14)                       /* use the hard‑coded values   */
            val = init[i - 1];
        else                               /* recurrence for i > 14      */
            val = 2 * cache[i - 1] + cache[i - 3] - 2 * cache[i - 4]
                 + 4 * cache[i - 5] + cache[i - 6] - cache[i - 9];
        cache[i] = val;
    }

    /* Update the highest known index (never decreases) */
    if (cache_len < (size_t)n)
        cache_len = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, return 0 */
    ensure_cache(n);
    return cache[(size_t)n];
}
