/*
 * A138633  Primes of the form 17*k - 9.
 * https://oeis.org/A138633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17*k - 9 where k is the n‑th integer ≥4 such that 17*k-9
 *            is prime
 * Range:     u8 n <= 3, u16 n <= 410, u32 n <= 45, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* static cache for the sequence terms */
static seq_t *cache      = NULL; /* array of cached terms */
static size_t cache_cap  = 0;    /* allocated capacity in terms of seq_t */
static idx_t  cache_len  = 0;    /* number of terms currently stored */
static idx_t  next_k     = 4;    /* next value of k to test */

/* enlarge the cache so it can hold at least 'need' terms */
static void grow_cache(idx_t need)
{
    if ((size_t)need <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < (size_t)need) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* allocation failed */
    cache     = tmp;
    cache_cap = new_cap;
}

/* ensure that cache contains at least 'n' terms (n ≥ 1) */
static void ensure_up_to(idx_t n)
{
    if (n <= cache_len) return;

    grow_cache(n);
    for (; cache_len < n; ++cache_len)
    {
        for (idx_t k = next_k; ; ++k)
        {
            /* compute value using a wider type to avoid overflow */
            uint64_t temp = 17ULL * k - 9ULL;
            seq_t val = (seq_t)temp;
            if (is_prime_big(val))
            {
                cache[cache_len] = val;
                next_k = k + 1;
                break;
            }
        }
    }
}

/* return the n‑th term (a(1) = 59, a(2) = 127, …) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 1, harmless */
    ensure_up_to(n);
    return cache[n - 1];              /* zero‑based array indexing */
}
