/*
 * A165382  Number of slanted n X 5 (i=1..n) X (j=i..5+i-1) 1..4 arrays with
 *          all 1s connected, all 2s connected, all 3s connected, all 4s
 *          connected, 1 in the upper left corner, 2 in the upper right
 *          corner, 3 in the lower left corner, 4 in the lower right corner,
 *          and with no element having more than 3 neighbo...
 * https://oeis.org/A165382
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2) - 3*a(n-4) + a(n-6) for n >= 13; initial values
 *            (offset 2) are a(2)=105, a(3)=1382, a(4)=6412, a(5)=18900,
 *            a(6)=40058, a(7)=70658, a(8)=110364, a(9)=159322, a(10)=217548,
 *            a(11)=284834, a(12)=361612
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache of already computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* capacity of the cache array */
static idx_t   cache_max = 0;  /* highest index stored (>=0) */

/* Initial values for indices 0..12 (indices <2 are unused but set to 0) */
static const seq_t init_vals[13] = {
    0ULL,     /* a(0) dummy */
    0ULL,     /* a(1) dummy */
    105ULL,   /* a(2) */
    1382ULL,  /* a(3) */
    6412ULL,  /* a(4) */
    18900ULL, /* a(5) */
    40058ULL, /* a(6) */
    70658ULL, /* a(7) */
    110364ULL,/* a(8) */
    159322ULL,/* a(9) */
    217548ULL,/* a(10) */
    284834ULL,/* a(11) */
    361612ULL /* a(12) */
};

/* Ensure that cache is allocated at least up to index n and that all
   terms up to n have been computed. */
static void ensure_cache(idx_t n)
{
    /* Allocate or grow cache if necessary */
    if (cache == NULL) {
        cache_cap = 13;                /* enough for indices 0..12 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return;           /* out of memory – caller will fail */
        for (idx_t i = 0; i < cache_cap; ++i) cache[i] = init_vals[i];
        cache_max = 12;
    }

    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;          /* out of memory – caller will fail */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i < 13) {
            /* Already initialized */
            continue;
        }
        seq_t a2 = cache[i - 2];
        seq_t a4 = cache[i - 4];
        seq_t a6 = cache[i - 6];
        seq_t diff = a2 - a4;
        seq_t val = diff * 3 + a6;
        cache[i] = val;
    }
    if (n > cache_max) cache_max = n;
}

/* Public function to return a(n) for n >= 2.
   For n < 2, the value is undefined – we return 0. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;
    ensure_cache(n);
    return cache[n];
}
