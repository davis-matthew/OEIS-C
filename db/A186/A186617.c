/*
 * A186617  Number of (n+5)X12 binary arrays with every 6X6 subblock commuting
 *          with each horizontal and vertical neighbor 6X6 subblock.
 * https://oeis.org/A186617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1)+a(n-4)-a(n-5)+a(n-6)-a(n-7)-a(n-10)+a(n-11)+a(n-12)-a(n-16)
 * for n > 24, with initial values:
 * a(1)=40150599, a(2)=955892, a(3)=1360475, a(4)=2676699, a(5)=4710363,
 * a(6)=6009659, a(7)=6820667, a(8)=6908987, a(9)=6919495, a(10)=6930031,
 * a(11)=6940831, a(12)=6954355, a(13)=6980043, a(14)=7071847, a(15)=7512023,
 * a(16)=8708687, a(17)=10602303, a(18)=13875155, a(19)=19034059, a(20)=24347399,
 * a(21)=31701828, a(22)=39823281, a(23)=48652486, a(24)=58874451
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed values, cache[0] unused */
static size_t cache_cap = 0;  /* current allocated capacity (in elements) */
static size_t last_filled = 0;/* largest index whose value is computed */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    /* We need capacity for indices 0..n */
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap) return;
    /* Grow capacity to the needed size (or a bit more for future expansions) */
    size_t new_cap = needed;
    if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) /* allocation failure – terminate */
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the cache with the first 24 values. */
static void init_cache(void)
{
    if (last_filled) return; /* already initialised */

    /* Allocate space for indices 0..24. */
    ensure_cache_size(24);

    cache[0] = (seq_t)0;
    cache[1] = (seq_t)40150599;
    cache[2] = (seq_t)955892;
    cache[3] = (seq_t)1360475;
    cache[4] = (seq_t)2676699;
    cache[5] = (seq_t)4710363;
    cache[6] = (seq_t)6009659;
    cache[7] = (seq_t)6820667;
    cache[8] = (seq_t)6908987;
    cache[9] = (seq_t)6919495;
    cache[10] = (seq_t)6930031;
    cache[11] = (seq_t)6940831;
    cache[12] = (seq_t)6954355;
    cache[13] = (seq_t)6980043;
    cache[14] = (seq_t)7071847;
    cache[15] = (seq_t)7512023;
    cache[16] = (seq_t)8708687;
    cache[17] = (seq_t)10602303;
    cache[18] = (seq_t)13875155;
    cache[19] = (seq_t)19034059;
    cache[20] = (seq_t)24347399;
    cache[21] = (seq_t)31701828;
    cache[22] = (seq_t)39823281;
    cache[23] = (seq_t)48652486;
    cache[24] = (seq_t)58874451;
    last_filled = 24;
}

/* Compute a(n) for n >= 1, lazily extending the cache as needed. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    init_cache();

    /* If we already have the value, return it immediately. */
    if ((size_t)n <= last_filled) return cache[(size_t)n];

    /* Ensure the array is large enough. */
    ensure_cache_size(n);

    /* Compute missing terms up to n. */
    for (idx_t i = (idx_t)(last_filled + 1); i <= n; ++i) {
        seq_t val = cache[(size_t)(i - 1)]   /* a(i-1) */
                  + cache[(size_t)(i - 4)]   /* a(i-4) */
                  - cache[(size_t)(i - 5)]   /* a(i-5) */
                  + cache[(size_t)(i - 6)]   /* a(i-6) */
                  - cache[(size_t)(i - 7)]   /* a(i-7) */
                  - cache[(size_t)(i -10)]  /* a(i-10) */
                  + cache[(size_t)(i -11)]  /* a(i-11) */
                  + cache[(size_t)(i -12)]  /* a(i-12) */
                  - cache[(size_t)(i -16)]; /* a(i-16) */
        cache[(size_t)i] = val;
    }
    last_filled = (size_t)n;
    return cache[(size_t)n];
}
