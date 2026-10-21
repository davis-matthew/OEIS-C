/*
 * A069059  Numbers k such that k and sigma(k) are not relatively prime.
 * https://oeis.org/A069059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th integer k satisfying gcd(k, σ(k)) > 1
 * Range:     u8 n <= 139, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array, seq_gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/sigma_of.h"
#include "../utils/seq_gcd.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache      = NULL;   /* holds a(1), a(2), … */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_len  = 0;      /* number of stored terms  */
static idx_t next_to_test = 1;     /* next integer to examine */

/* Ensure the cache can hold at least 'need' terms. */
static void grow_cache(size_t need)
{
    if (cache_cap >= need)
        return;

    size_t new_cap = (cache_cap < 16 ? 16 : cache_cap);
    while (new_cap < need)
        new_cap <<= 1;          /* double until we have enough */

    seq_t *new_cache = seq_alloc_new_array(new_cap);
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    if (cache != NULL)
        seq_free_array(cache);

    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n<1 – harmless */
        return (seq_t)0;

    while (cache_len < (size_t)n)
    {
        seq_t sigma = sigma_of(next_to_test);               /* σ(k) */
        seq_t g     = seq_gcd((seq_t)next_to_test, sigma);  /* gcd(k,σ(k)) */

        if (g > (seq_t)1)        /* condition satisfied */
        {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_to_test;
        }
        ++next_to_test;
    }

    return cache[n - 1];
}
