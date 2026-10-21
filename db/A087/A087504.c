/*
 * A087504  Composition length of the n-th symmetric power of the natural
 *          representation of a finite subgroup of SL(2,C) of type E_7 (binary
 *          octahedral group).
 * https://oeis.org/A087504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) – a(n-3) + 2·a(n-4) – a(n-5) + a(n-7) – a(n-8)
 *            with a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=2, a(5)=2, a(6)=3,
 *            a(7)=3
 * Range:     u8 n <= 611, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;     /* array of computed terms */
static size_t cap   = 0;        /* allocated number of elements */
static size_t size  = 0;        /* how many terms are already present (highest+1) */
static bool    init  = false;  /* whether the first eight terms are set up */

/* initialise the first eight terms */
static void init_cache(void)
{
    if (cap < 8) {
        cap = 8;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!cache) abort();
    }
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)1;
    cache[4] = (seq_t)2;
    cache[5] = (seq_t)2;
    cache[6] = (seq_t)3;
    cache[7] = (seq_t)3;
    size  = 8;
    init  = true;
}

/* make sure all terms up to and including n are available in cache */
static void ensure_cache(idx_t n)
{
    size_t target = (size_t)n;

    if (!init) init_cache();

    if (target < 8) return;         /* all needed terms already initialised */

    /* grow the cache if necessary */
    if (cap <= target) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap <= target) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (size_t i = size; i <= target; ++i) {
        seq_t a1 = cache[i-1];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4] << 1;   /* 2·a(n-4) */
        seq_t a5 = cache[i-5];
        seq_t a7 = cache[i-7];
        seq_t a8 = cache[i-8];
        cache[i] = a1 - a3 + a4 - a5 + a7 - a8;
    }
    size = target + 1;
}

/* public sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
