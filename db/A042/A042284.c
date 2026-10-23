/*
 * A042284  Numerators of continued fraction convergents to sqrt(668).
 * https://oeis.org/A042284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 112894*a(n-8) - a(n-16) with initial values a(0)=25,
 *            a(1)=26, a(2)=155, a(3)=336, a(4)=4187, a(5)=8710, a(6)=47737,
 *            a(7)=56447, a(8)=2870087, … as listed
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Static cache for memoization */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;
static bool init_done = false;

/* Integer square root for idx_t (used only in the initialization) */
static idx_t int_sqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) r++;
    while (r * r > n) r--;
    return r;
}

/* Build the first 16 terms using the continued‑fraction convergents of sqrt(668) */
static void init_sequence(void)
{
    if (init_done) return;
    init_done = true;

    const idx_t D = 668;
    const idx_t a0 = int_sqrt(D);            /* 25 */

    /* Find the continued fraction period of sqrt(D) */
    const idx_t MAXPER = 64;                  /* safe upper bound for sqrt(668) */
    idx_t period[MAXPER];
    size_t plen = 0;

    idx_t m = 0, d = 1, a = a0;

    while (true) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        period[plen++] = a;
        if (a == 2 * a0 || plen >= MAXPER) break;   /* period finished or safety */
    }

    /* Compute the first 16 convergent numerators */
    seq_t p_m2 = 0;    /* p_{-2} */
    seq_t p_m1 = 1;    /* p_{-1} */
    seq_t init[16];

    for (size_t k = 0; k < 16; ++k) {
        seq_t ak = (k == 0) ? (seq_t)a0 : (seq_t)period[(k - 1) % plen];
        init[k] = ak * p_m1 + p_m2;
        p_m2 = p_m1;
        p_m1 = init[k];
    }

    /* Allocate the cache and store the initial terms */
    cache_cap = 32;                           /* small initial capacity */
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) return;                       /* allocation failure – nothing to do */

    for (size_t i = 0; i < 16; ++i)
        cache[i] = init[i];
    cache_size = 16;
}

seq_t a(idx_t n)
{
    init_sequence();

    size_t idx = (size_t)n;
    if (idx < cache_size) return cache[idx];

    /* Grow cache to accommodate index */
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (idx >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure – fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (size_t i = cache_size; i <= idx; ++i) {
        /* a(i) = 112894*a(i-8) - a(i-16) */
        seq_t term = (seq_t)112894 * cache[i - 8] - cache[i - 16];
        cache[i] = term;
    }
    cache_size = idx + 1;
    return cache[idx];
}
