/*
 * A052664  E.g.f. (1-x)/(1-2x-3x^2+3x^3).
 * https://oeis.org/A052664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A052664
 * E.g.f.: (1-x)/(1-2x-3x^2+3x^3)
 * Recurrence: a(0)=1, a(1)=1, a(2)=10
 *   (3n^3+18n^2+33n+18)*a(n) + (18+3n^2+15n)*a(n+1) + (2n+6)*a(n+2) - a(n+3) = 0
 * This implementation computes a(n) by iterating the recurrence and caching
 * previously computed values. It uses the utility functions
 * seq_alloc_new_array and free_seq_array to manage the cache.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of elements currently stored */
static idx_t cache_cap = 0;   /* capacity of the cache */

/* Ensure that the cache holds at least 'needed' elements.
 * Resizes the cache if necessary, preserving existing values. */
static void ensure_cache_cap(idx_t needed)
{
    if (needed <= cache_cap)
        return;

    idx_t new_cap = needed * 2 + 10;
    seq_t *new_mem = seq_alloc_new_array(new_cap);
    if (!new_mem)
        abort();   /* memory allocation failed */

    /* Copy current values to the new array */
    for (idx_t i = 0; i < cache_len; ++i)
        new_mem[i] = cache[i];

    /* Free the old array and update pointers */
    free_seq_array(cache);
    cache = new_mem;
    cache_cap = new_cap;
}

/* Initialise the cache with the first three terms of the sequence. */
static void init_cache(void)
{
    if (cache_len > 0)
        return;

    ensure_cache_cap(3);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)10;
    cache_len = 3;
}

/* Return the n-th term of OEIS A052664 (n ≥ 0). */
seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly. */
    if (n == 0 || n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)10;

    init_cache();

    if (n < cache_len)
        return cache[n];

    /* Compute terms up to n, extending the cache as needed. */
    for (idx_t k = cache_len; k <= n; ++k)
    {
        idx_t m = k - 3;       /* recurrence uses a(m), a(m+1), a(m+2) */

        seq_t a0 = cache[m];
        seq_t a1 = cache[m + 1];
        seq_t a2 = cache[m + 2];

        /* Compute coefficients using unsigned 64‑bit arithmetic. */
        unsigned long long c0 = 3ULL * m * m * m + 18ULL * m * m + 33ULL * m + 18ULL;
        unsigned long long c1 = 18ULL + 3ULL * m * m + 15ULL * m;
        unsigned long long c2 = 2ULL * m + 6ULL;

        seq_t coeff0 = (seq_t)c0;
        seq_t coeff1 = (seq_t)c1;
        seq_t coeff2 = (seq_t)c2;

        seq_t term = coeff1 * a1 + coeff2 * a2 - coeff0 * a0;

        ensure_cache_cap(k + 1);
        cache[k] = term;
    }

    cache_len = n + 1;
    return cache[n];
}
