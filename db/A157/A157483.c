/*
 * A157483  Numbers k such that k-1 and k+1 are divisible by exactly 3 primes,
 *          counted with multiplicity.
 * https://oeis.org/A157483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 3430, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A157483: Numbers k such that Ω(k−1) = Ω(k+1) = 3.
   The function performs an on‑demand search, keeping a static cache
   of found terms.  Omega is computed by factoring k−1 and k+1
   with a 64‑bit trial‑division routine.  This method runs in
   O(n log n) time for the requested range and uses only standard
   C library facilities. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

/* Cache of computed terms. */
static seq_t *cache      = NULL;   /* array of k values */
static size_t cache_len  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* current capacity */

/* Last integer that was examined by the search. */
static idx_t last_checked = 2;     /* start at 2 so first increment gives 3 */

/* Resize the cache if needed. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Compute Ω(n) for a 64‑bit integer using factor_u64. */
static int omega_uint64(uint64_t n)
{
    if (n <= 1)
        return 0;
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);
    int omega = 0;
    for (size_t i = 0; i < cnt; ++i)
        omega += (int)ex[i];
    return omega;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for n=0, but safe */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Search for additional terms until we have at least n entries. */
    while (cache_len < (size_t)n)
    {
        ++last_checked;

        uint64_t km1 = (uint64_t)last_checked - 1;
        uint64_t kp1 = (uint64_t)last_checked + 1;

        if (omega_uint64(km1) == 3 && omega_uint64(kp1) == 3)
        {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[(size_t)n - 1];
}
