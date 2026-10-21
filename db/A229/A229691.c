/*
 * A229691  Number of defective 3-colorings of an nX5 0..2 array connected
 *          horizontally and antidiagonally with exactly two mistakes, and
 *          colors introduced in row-major 0..2 order.
 * https://oeis.org/A229691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 45*a(n-1) -729*a(n-2) +4995*a(n-3) -13122*a(n-4)
 *            +14580*a(n-5) -5832*a(n-6) for n>7; a(1)=12, a(2)=1171,
 *            a(3)=35825, a(4)=848937, a(5)=17724789, a(6)=342532665,
 *            a(7)=6279934941
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t INIT[7] = {
    (seq_t)12,      /* a(1) */
    (seq_t)1171,    /* a(2) */
    (seq_t)35825,   /* a(3) */
    (seq_t)848937,  /* a(4) */
    (seq_t)17724789,/* a(5) */
    (seq_t)342532665,/* a(6) */
    (seq_t)6279934941/* a(7) */
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated entries */
static idx_t   cache_max = 0;  /* highest n computed so far (1‑based) */

seq_t a(idx_t n) {
    /* Ensure cache is large enough */
    if (n > (idx_t)cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 7;
        while ((idx_t)new_cap < n) new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise first 7 terms if not already done */
    if (cache_max < 7) {
        size_t limit = (cache_cap < 7) ? cache_cap : 7;
        for (size_t i = 0; i < limit; ++i) cache[i] = INIT[i];
        cache_max = (cache_max < (idx_t)limit) ? limit : cache_max;
    }

    /* If the requested term is already computed, return it */
    if (n <= (idx_t)cache_max) return cache[n-1];

    /* Compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* i is 1‑based; cache indexes are 0‑based */
        seq_t pos = cache[i-2]*45   + cache[i-4]*4995   + cache[i-6]*14580;
        seq_t neg = cache[i-3]*729  + cache[i-5]*13122  + cache[i-7]*5832;
        cache[i-1] = pos - neg;   /* pos >= neg by recurrence definition */
    }

    cache_max = n;
    return cache[n-1];
}
