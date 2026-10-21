/*
 * A227273  Expansion of 1/((1-x)*(1-4*x)*(1-10*x)*(1-20*x)*(1-35*x)).
 * https://oeis.org/A227273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A227273: Expansion of 1/((1-x)*(1-4*x)*(1-10*x)*(1-20*x)*(1-35*x)).
   Recurrence: a(0)=1, a(1)=70, a(2)=3321, a(3)=135450, a(4)=5143341
   For n≥5:   a(n) = 70*a(n‑1) - 1579*a(n‑2) + 13510*a(n‑3)
               - 40000*a(n‑4) + 28000*a(n‑5) */
#include <stdlib.h>

static seq_t *cache      = NULL;
static size_t cache_size = 0;
static idx_t   cache_len  = 0;

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_size)
        return;
    size_t new_cap = cache_size ? cache_size * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache      = new_ptr;
    cache_size = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise cache with the first five terms */
    if (cache_len == 0) {
        ensure_capacity(5);
        cache[0] =  (seq_t)1;
        cache[1] =  (seq_t)70;
        cache[2] =  (seq_t)3321;
        cache[3] =  (seq_t)135450;
        cache[4] =  (seq_t)5143341;
        cache_len = 5;
    }

    if (n < cache_len)
        return cache[(size_t)n];

    /* Extend the cache up to the required index */
    size_t target = (size_t)n;
    ensure_capacity(target + 1);

    for (size_t i = cache_len; i <= target; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        /* Recurrence: a(n) = 70*a(n-1) - 1579*a(n-2) + 13510*a(n-3)
                          - 40000*a(n-4) + 28000*a(n-5) */
        seq_t val = (seq_t)70 * a1
                  - (seq_t)1579 * a2
                  + (seq_t)13510 * a3
                  - (seq_t)40000 * a4
                  + (seq_t)28000 * a5;
        cache[i] = val;
    }

    cache_len = target + 1;
    return cache[(size_t)n];
}
