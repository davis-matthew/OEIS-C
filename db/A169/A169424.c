/*
 * A169424  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the generating function (1 + 2 * Σ_{k=1}^{31}
 *            x^k + x^32) / (1 – 25 Σ_{k=1}^{31} x^k + 325 x^32). Recurrence:
 *            a(0) = 1 for 1 ≤ n ≤ 31 : a(n) = 25 * Σ_{k=1}^{n} a(n‑k) + 2
 *            a(32) : a(32) = 25 * Σ_{k=1}^{31} a(32‑k) – 325*a(0) + 1 for n ≥
 *            33 : a(n) = 25 * Σ_{k=1}^{31} a(n‑k) – 325*a...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* stored sequence values   */
static idx_t cache_size = 0;          /* allocated array length   */
static idx_t computed_max = 0;        /* last index for which value is valid */
static bool cache_initialized = false;

/* Ensure the cache can hold indices up to n */
static void ensure_cache_size(idx_t n)
{
    if (n < cache_size)
        return;
    idx_t new_size = (cache_size == 0) ? 1 : cache_size;
    while (new_size <= n)
        new_size <<= 1;                     /* double until sufficient */

    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* out of memory – terminate */
        exit(1);
    }
    /* initialise new elements to zero */
    for (idx_t i = cache_size; i < new_size; ++i)
        tmp[i] = (seq_t)0;
    cache = tmp;
    cache_size = new_size;
}

/* compute a(n) ----------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_cache_size(n);

    /* initialise a(0) if not done yet */
    if (!cache_initialized) {
        cache[0] = (seq_t)1;
        computed_max = 0;
        cache_initialized = true;
    }

    if (n <= computed_max)
        return cache[n];

    /* compute missing terms one by one */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t value = (seq_t)0;
        seq_t sum = (seq_t)0;

        if (i == 0) {
            value = (seq_t)1;            /* already set but keep for completeness */
        } else if (i <= 31) {
            /* sum of a(i-1) .. a(0) */
            for (idx_t k = 1; k <= i; ++k)
                sum += cache[i - k];
            value = (seq_t)25 * sum + (seq_t)2;
        } else if (i == 32) {
            for (idx_t k = 1; k <= 31; ++k)
                sum += cache[i - k];
            value = (seq_t)25 * sum - (seq_t)325 * cache[0] + (seq_t)1;
        } else {   /* i >= 33 */
            for (idx_t k = 1; k <= 31; ++k)
                sum += cache[i - k];
            value = (seq_t)25 * sum - (seq_t)325 * cache[i - 32];
        }

        cache[i] = value;
    }

    computed_max = n;
    return cache[n];
}
