/*
 * A082028  Expansion of exp(x)*(1+x)/(1-x)^2.
 * https://oeis.org/A082028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1, a(1) = 4, a(2) = 17
   a(n) = ((2n+7)·a(n‑1) – 5n·a(n‑2) + 3(n‑2)·a(n‑3)) / 2   (n ≥ 3)   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache holds all terms that have been computed so far.
       cache[i] == a(i) for 0 ≤ i < cache_size. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;

    /* Initialisation: allocate a small cache and store the three seed values. */
    if (cache_cap == 0) {
        cache_cap = 8;                     /* reserve a few extra slots */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(1);        /* in the unlikely event of OOM */
        cache[0] = (seq_t)1;               /* a(0) */
        cache[1] = (seq_t)4;               /* a(1) */
        cache[2] = (seq_t)17;              /* a(2) */
        cache_size = 3;
    }

    /* If the requested term is already cached, return it immediately. */
    if (n < cache_size)
        return cache[(size_t)n];

    /* Grow the cache as far as needed, recomputing terms on the way. */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* Ensure enough space for the new term. */
        if ((size_t)i + 1 > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < cache_cap + 1) new_cap = cache_cap + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }

        seq_t term1 = (seq_t)(2 * i + 7) * cache[i - 1];
        seq_t term2 = (seq_t)(5 * i) * cache[i - 2];
        seq_t term3 = (seq_t)(3 * (i - 2)) * cache[i - 3];
        seq_t num   = term1 - term2 + term3;
        cache[i] = num / (seq_t)2; /* division is exact by the recurrence */
    }

    cache_size = n + 1;
    return cache[(size_t)n];
}
