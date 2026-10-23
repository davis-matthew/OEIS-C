/*
 * A135094  a(n) = 2*a(n-1) + 2*a(n-2) - 4*a(n-3) with n>2, a(0)=0, a(1)=1,
 *          a(2)=3.
 * https://oeis.org/A135094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1, a(2) = 3 a(n) = 2*a(n-1) + 2*a(n-2) -
 *            4*a(n-3) for n > 2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* array of already computed terms */
static idx_t  cache_size = 0;    /* current allocated size */
static idx_t  computed_upto = 0; /* largest index computed */
static bool   init_done = false;

/* Initialise the cache with the first three terms. */
static void
init_cache(void)
{
    if (init_done) return;
    cache_size = 3;
    cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)3;
    computed_upto = 2;
    init_done = true;
}

/* Grow the cache to accommodate indices up to n. */
static void
ensure_capacity(idx_t n)
{
    if (n < cache_size) return;
    idx_t new_size = n + 1;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache_size = new_size;
}

/* Compute and return a(n). */
seq_t a(idx_t n)
{
    init_cache();

    if (n <= computed_upto) {
        return cache[n];
    }

    ensure_capacity(n);

    /* Compute terms from computed_upto+1 up to n. */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        seq_t term = (cache[i-1] * (seq_t)2) +
                    (cache[i-2] * (seq_t)2) -
                    (cache[i-3] * (seq_t)4);
        cache[i] = term;
    }
    computed_upto = n;
    return cache[n];
}
