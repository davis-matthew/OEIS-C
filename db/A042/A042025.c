/*
 * A042025  Denominators of continued fraction convergents to sqrt(536).
 * https://oeis.org/A042025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 291850*a(n-10) - a(n-20) (n > 19) with initial values
 *            a(0)=1, a(1)=6, a(2)=7, a(3)=13, a(4)=33, a(5)=178, a(6)=389,
 *            a(7)=567, a(8)=956, a(9)=6303, a(10)=290894, a(11)=1751667,
 *            a(12)=2042561, a(13)=3794228, a(14)=9631017, a(15)=51949313,
 *            a(16)=113529643, a(17)=165478956, a(18)=279008599, a(...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base[20] = {
    (seq_t)1, (seq_t)6, (seq_t)7, (seq_t)13, (seq_t)33,
    (seq_t)178, (seq_t)389, (seq_t)567, (seq_t)956, (seq_t)6303,
    (seq_t)290894, (seq_t)1751667, (seq_t)2042561, (seq_t)3794228,
    (seq_t)9631017, (seq_t)51949313, (seq_t)113529643, (seq_t)165478956,
    (seq_t)279008599, (seq_t)1839530550
};

static seq_t *seq_cache = NULL;   /* cached values */
static size_t cache_cap = 0;      /* allocated capacity in elements */
static idx_t  max_defined = 0;    /* largest index already computed */

/* Ensure the cache can hold at least needed elements (zero‑based) */
static void ensure_capacity(size_t needed)
{
    if (cache_cap > needed) return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap <= needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                /* out of memory: abort */
    seq_cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (!seq_cache) {
        /* we need at least 20 elements for the seed */
        ensure_capacity(20);
        for (idx_t i = 0; i < 20; ++i)
            seq_cache[i] = base[i];
        max_defined = 19;
    }

    if (n <= max_defined)
        return seq_cache[n];

    /* extend cache to cover index n */
    ensure_capacity((size_t)n);

    for (idx_t i = max_defined + 1; i <= n; ++i) {
        /* i is always ≥20 here */
        seq_cache[i] = (seq_t)291850 * seq_cache[i - 10] - seq_cache[i - 20];
    }
    max_defined = n;
    return seq_cache[n];
}
