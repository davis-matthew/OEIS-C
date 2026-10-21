/*
 * A205165  Number of (n+1)X4 0..3 arrays with every 2X2 subblock having
 *          unequal diagonal elements or unequal antidiagonal elements, and
 *          new values 0..3 introduced in row major order.
 * https://oeis.org/A205165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 224*a(n-1) - 2803*a(n-2) + 11196*a(n-3) - 16677*a(n-4) +
 *            9438*a(n-5) - 1728*a(n-6) Initial values: a(1) = 2277, a(2) =
 *            474853, a(3) = 100138314, a(4) = 21125224557, a(5) =
 *            4456641224469, a(6) = 940186880752830
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* 1‑based array, cache[0] unused */
static size_t cap = 0;          /* allocated size of cache */
static idx_t max_comp = 0;      /* highest index computed so far */

static void
ensure_capacity(idx_t needed)
{
    if ((size_t)needed < cap) return;
    size_t new_cap = ((size_t)needed) + 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return;             /* leave cache unchanged on failure */
    if (cache) {                         /* copy existing values */
        for (size_t i = 0; i < cap; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cap   = new_cap;
}

static void
init_base(void)
{
    if (max_comp >= 6) return;
    ensure_capacity(6);
    cache[1] = (seq_t)2277ULL;
    cache[2] = (seq_t)474853ULL;
    cache[3] = (seq_t)100138314ULL;
    cache[4] = (seq_t)21125224557ULL;
    cache[5] = (seq_t)4456641224469ULL;
    cache[6] = (seq_t)940186880752830ULL;
    max_comp = 6;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    init_base();
    if (n <= max_comp) return cache[n];
    ensure_capacity(n);
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t t1 = cache[i-1] * (seq_t)224U;
        seq_t t2 = cache[i-2] * (seq_t)2803U;
        seq_t t3 = cache[i-3] * (seq_t)11196U;
        seq_t t4 = cache[i-4] * (seq_t)16677U;
        seq_t t5 = cache[i-5] * (seq_t)9438U;
        seq_t t6 = cache[i-6] * (seq_t)1728U;
        cache[i] = t1 - t2 + t3 - t4 + t5 - t6;
    }
    max_comp = n;
    return cache[n];
}
