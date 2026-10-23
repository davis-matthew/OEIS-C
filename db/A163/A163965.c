/*
 * A163965  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A163965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 17, a(2) = 272, a(3) = 4352, a(4) = 69632, a(5)
 *            = 1114112 For n ≥ 6 : a(n) = -120·a(n‑6) + 15·∑_{k=1..5} a(n‑k)
 *            (for n = 6 the numerator term of the generating function adds
 *            +1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t computed_up_to = (idx_t)-1;   /* last index with a value stored */

static void grow_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_size < needed)
    {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_size = needed;
    }
}

static void init_base(void)
{
    /* Ensure cache holds indices 0..6 and initialise them. */
    grow_cache(6);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)17;
    cache[2] = (seq_t)272;
    cache[3] = (seq_t)4352;
    cache[4] = (seq_t)69632;
    cache[5] = (seq_t)1114112;

    /* a(6) is special because of the +1 term coming from the numerator. */
    seq_t sum = cache[5] + cache[4] + cache[3] + cache[2] + cache[1];
    cache[6] = (seq_t)1 + (seq_t)15 * sum - (seq_t)120 * cache[0];
    computed_up_to = 6;
}

static void compute_up_to(idx_t n)
{
    if (computed_up_to >= n) return;

    for (idx_t i = computed_up_to + 1; i <= n; ++i)
    {
        seq_t sum = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4] + cache[i-5];
        cache[i] = (seq_t)15 * sum - (seq_t)120 * cache[i-6];
    }
    computed_up_to = n;
}

seq_t a(idx_t n)
{
    if (cache == NULL) init_base();
    grow_cache(n);
    compute_up_to(n);
    return cache[n];
}
