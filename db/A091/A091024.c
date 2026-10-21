/*
 * A091024  Let v(0) be the column vector (1,0,0,0)'; for n>0, let v(n) = [1 1
 *          1 1 / 1 1 1 0 / 1 1 0 0/ 1 0 0 0] v(n-1). Sequence gives third
 *          entry of v(n).
 * https://oeis.org/A091024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - a(n-3) - a(n-4) Initial conditions:
 *            a(0)=0, a(1)=1, a(2)=2, a(3)=7
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t allocated = 0;      /* allocated array size */
static idx_t computed = 3;        /* highest index whose value is computed */

/* Initialise the cache with the first four terms */
static void init_cache(void)
{
    allocated = 4;
    cache = (seq_t *)malloc(sizeof(seq_t) * allocated);
    if (!cache) abort(); /* if allocation fails */
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)7;
}

/* Expand the cache capacity to at least new_cap */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= allocated)
        return;
    size_t cap = allocated ? allocated : 4;
    while (cap < new_cap) {
        size_t nxt = cap * 2;
        if (nxt < cap) nxt = new_cap; /* overflow protection */
        if (nxt < new_cap) nxt = new_cap;
        cap = nxt;
    }
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cap);
    if (!tmp) abort();
    cache = tmp;
    allocated = cap;
}

/* Compute terms up to n */
static void compute_to(idx_t n)
{
    if (n <= computed) return;
    ensure_capacity((size_t)n + 1);
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* recurrence: a(i) = 2*a(i-1) + 3*a(i-2) - a(i-3) - a(i-4) */
        seq_t val = (seq_t)2 * cache[(size_t)i - 1]
                    + (seq_t)3 * cache[(size_t)i - 2]
                    - cache[(size_t)i - 3]
                    - cache[(size_t)i - 4];
        cache[(size_t)i] = val;
    }
    computed = n;
}

seq_t a(idx_t n)
{
    if (!cache) init_cache();        /* first call */
    compute_to(n);                   /* ensure value exists */
    return cache[(size_t)n];
}
