/*
 * A211217  Maximum of sigma(x) * sigma(y), where x + y = n.
 * https://oeis.org/A211217
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: maximum of sigma(x) * sigma(y) with x + y = n
 * a(n) = max_{1 <= x < n} sigma(x) * sigma(n - x),  for n >= 2
 */
#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_seq_array.h"   /* seq_t *alloc_seq_array(size_t n); */
#include "../utils/free_cache.h"       /* void free_cache(seq_t *p); */

static seq_t *sigma_cache = NULL; /* sum-of-divisors cache */
static size_t sigma_cap = 0;     /* cache size (number of slots) */

static seq_t *max_cache = NULL;  /* a(n) cache */
static size_t max_cap = 0;       /* cache size */

/* Rebuild sigma_cache to hold values for 0 .. up_to inclusive. */
static void recompute_sigma(idx_t up_to)
{
    size_t n = (size_t)up_to;
    seq_t *new_arr = alloc_seq_array(n + 1);

    /* initialize to zero */
    for (size_t i = 0; i <= n; ++i)
        new_arr[i] = 0;

    /* classic divisor‑sum sieve */
    for (size_t i = 1; i <= n; ++i) {
        seq_t si = (seq_t)i;
        for (size_t j = i; j <= n; j += i)
            new_arr[j] += si;
    }

    if (sigma_cache)
        free_cache(sigma_cache);
    sigma_cache = new_arr;
    sigma_cap   = n + 1;
}

/* Ensure sigma_cache contains values up to n. */
static void ensure_sigma(idx_t n)
{
    if (sigma_cap <= (size_t)n)
        recompute_sigma(n);
}

/* Compute a(n) and store it in max_cache[n], expanding cache if needed. */
static void compute_max_n(idx_t n)
{
    if (n < 2)
        return;                       /* nothing to compute */

    ensure_sigma(n);

    seq_t best = 0;
    for (idx_t x = 1; x < n; ++x) {
        seq_t prod = sigma_cache[x] * sigma_cache[n - x];
        if (prod > best)
            best = prod;
    }

    if (max_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_arr = alloc_seq_array(new_cap);

        /* copy existing values */
        if (max_cache) {
            memcpy(new_arr, max_cache, sizeof(seq_t) * max_cap);
            free_cache(max_cache);
        }

        /* initialise new slots to 0 */
        for (size_t i = max_cap; i < new_cap; ++i)
            new_arr[i] = 0;

        max_cache = new_arr;
        max_cap   = new_cap;
    }

    max_cache[n] = best;
}

/* Public entry point */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                       /* undefined for n < 2 */

    if (max_cap <= (size_t)n || max_cache[n] == 0)
        compute_max_n(n);

    return max_cache[n];
}
