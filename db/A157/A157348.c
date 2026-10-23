/*
 * A157348  Positive numbers y such that y^2 is of the form x^2+(x+281)^2 with
 *          integer x.
 * https://oeis.org/A157348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n‑3) – a(n‑6) for n > 6; a(1) = 229, a(2) = 281, a(3)
 *            = 365, a(4) = 1009, a(5) = 1405, a(6) = 1961
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* cache for computed values (1‑based) */
static seq_t *cache = NULL;
static idx_t   cap   = 0;       /* capacity of the allocated array */
static idx_t   cached_max = 0;  /* highest index for which a value is known */

static void ensure_cache_size(idx_t need)
{
    if (cap <= need) {
        idx_t new_cap = cap > 0 ? cap * 2 : 16;
        while (new_cap <= need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)new_cap);
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = cap; i < new_cap; ++i) cache[i] = (seq_t)0;
        cap = new_cap;
    }
}

static void init_cache(void)
{
    if (cache == NULL) {
        ensure_cache_size(6);           /* need indices 0..6 */
        cache[0] = (seq_t)0;            /* unused */
        cache[1] = (seq_t)229;
        cache[2] = (seq_t)281;
        cache[3] = (seq_t)365;
        cache[4] = (seq_t)1009;
        cache[5] = (seq_t)1405;
        cache[6] = (seq_t)1961;
        cached_max = 6;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1, but safe */
    if (cache == NULL) init_cache();

    if (n <= cached_max) return cache[n];

    ensure_cache_size(n);
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        /* recurrence only applies for i > 6; i will always be > 6 here */
        seq_t val = 6 * cache[i - 3];
        val = val - cache[i - 6];
        cache[i] = val;
    }
    cached_max = n;
    return cache[n];
}
