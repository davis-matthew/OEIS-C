/*
 * A163954  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A163954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -36*a(n-6) + 8*Sum_{k=1..5} a(n-k) with initial values
 *            a(0)=1, a(1)=10, a(2)=90, a(3)=810, a(4)=7290, a(5)=65610,
 *            a(6)=590445. The recurrence holds for n>=7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t computed_max = 0;  /* largest index for which cache[i] is valid */

/* Ensure cache has space for indices 0..need-1.  */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return; /* crash on allocation failure */
    if (cache)
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialize the first seven terms if not already done.  */
static void init_initial_terms(void)
{
    static const seq_t init[7] = {1, 10, 90, 810, 7290, 65610, 590445};
    ensure_cache(7);
    if (computed_max < 6) {
        for (size_t i = 0; i <= 6; ++i)
            cache[i] = init[i];
        computed_max = 6;
    }
}

seq_t a(idx_t n)
{
    /* Handle negative indices gracefully: sequence is undefined below offset. */
    if (n == (idx_t)-1) /* just to avoid implicit warning, but seq_t returns 0 */
        return (seq_t)0;

    /* Ensure cache covers n and has initial terms. */
    init_initial_terms();
    if (n > (idx_t)computed_max) {
        ensure_cache((size_t)n + 1);
        /* Compute values up to n.  For i <=6 the values are already set. */
        for (size_t i = computed_max + 1; i <= (size_t)n; ++i) {
            /* Recurrence holds for n >= 7, i.e. i >= 7. */
            seq_t sum = cache[i - 1];
            sum += cache[i - 2];
            sum += cache[i - 3];
            sum += cache[i - 4];
            sum += cache[i - 5];
            sum *= (seq_t)8;
            cache[i] = sum - (cache[i - 6] * (seq_t)36);
        }
        computed_max = (size_t)n;
    }
    return cache[(size_t)n];
}
