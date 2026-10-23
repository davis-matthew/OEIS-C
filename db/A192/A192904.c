/*
 * A192904  Constant term in the reduction by (x^2 -> x + 1) of the polynomial
 *          p(n,x) defined below at Comments.
 * https://oeis.org/A192904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-3) + a(n-4); Initial values: a(0)=1,
 *            a(1)=0, a(2)=1, a(3)=5
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* array of computed terms           */
static size_t cache_size = 0;        /* current allocated capacity        */
static size_t computed_upto = (size_t)-1; /* highest index already computed */

/* lazy initialisation of the first four terms */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    /* allocate space for the first 4 terms */
    cache_size = 4;
    cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    if (!cache)
        exit(EXIT_FAILURE);

    cache[0] = 1;   /* a(0) */
    cache[1] = 0;   /* a(1) */
    cache[2] = 1;   /* a(2) */
    cache[3] = 5;   /* a(3) */

    computed_upto = 3;  /* last computed index */
}

/* grow the cache until it can hold at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (cache_size >= needed)
        return;

    size_t new_cap = cache_size ? cache_size : 4;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);

    cache = new_cache;
    cache_size = new_cap;
}

seq_t a(idx_t n)
{
    init_cache();

    size_t idx = (size_t)n;
    ensure_capacity(idx + 1);

    if (idx > computed_upto) {
        size_t start = computed_upto + 1;
        if (start < 4)
            start = 4;      /* indices <4 were already initialized */

        for (size_t i = start; i <= idx; ++i)
            cache[i] = 3 * cache[i - 1] + cache[i - 3] + cache[i - 4];

        computed_upto = idx;
    }

    return cache[idx];
}
