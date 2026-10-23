/*
 * A122115  a(n) = a(n-1) + a(n-3) + a(n-5).
 * https://oeis.org/A122115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-5), with a(1) = -3, a(2) = -1, a(3)
 *            = 4, a(4) = 8, a(5) = 15
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 47, i64 n <= 96, i128 n <= 194,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap   = 0;

/* Ensure the cache contains the first five terms. */
static void cache_init(void)
{
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)(-3);
        cache[1] = (seq_t)(-1);
        cache[2] = (seq_t)(4);
        cache[3] = (seq_t)(8);
        cache[4] = (seq_t)(15);
        cache_size = 5;
    }
}

/* Grow the cache to accommodate at least 'needed' elements. */
static void cache_grow_to(idx_t needed)
{
    if (needed <= cache_cap) return;
    while (cache_cap < needed) cache_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n < 1, return 0 */

    cache_init();

    if (n <= cache_size) return cache[n-1];

    idx_t new_size = n;
    cache_grow_to(new_size);

    for (idx_t i = cache_size; i < new_size; ++i) { /* i is 0‑based index for term n = i+1 */
        idx_t n_term = i + 1;
        seq_t val;
        if (n_term == 1)       val = (seq_t)(-3);
        else if (n_term == 2)  val = (seq_t)(-1);
        else if (n_term == 3)  val = (seq_t)(4);
        else if (n_term == 4)  val = (seq_t)(8);
        else if (n_term == 5)  val = (seq_t)(15);
        else {
            /* a(n) = a(n-1) + a(n-3) + a(n-5) */
            val = cache[n_term-2] + cache[n_term-4] + cache[n_term-6];
        }
        cache[i] = val;
    }

    cache_size = new_size;
    return cache[n-1];
}
