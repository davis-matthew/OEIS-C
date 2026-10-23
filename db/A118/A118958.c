/*
 * A118958  Primes that cannot be written as 2^k + p with p prime < 2^k.
 * https://oeis.org/A118958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 4651, u32 n <= 5002, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A118958 – primes that cannot be written as 2^k + p with
    p prime < 2^k.
    The implementation searches successive primes and tests the
    defining property.  All found terms are stored in a static cache
    so that any later call with the same or a larger index can
    answer in O(1).  The algorithm is valid for both C11 and C++17.
    It uses the utility function is_prime_big(seq_t) for primality.
*/

#include <stddef.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache_vec;
static seq_t last_checked = 1;
#else
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t last_checked = 1;
#endif

/* add a new term to the cache */
static void add_to_cache(seq_t val)
{
#ifdef __cplusplus
    cache_vec.push_back(val);
#else
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;               /* allocation failed – abort */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
#endif
}

/* current cache size */
static size_t cache_size(void)
{
#ifdef __cplusplus
    return cache_vec.size();
#else
    return cache_len;
#endif
}

/* retrieve element at zero‑based index */
static seq_t cache_get(size_t idx)
{
#ifdef __cplusplus
    return cache_vec[idx];
#else
    return cache[idx];
#endif
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    /* Generate terms until we have enough */
    while (cache_size() < n) {
        /* move to the next candidate prime */
        do
            last_checked++;
        while (last_checked > 2 && (last_checked & 1) == 0);      /* skip evens >2 */

        if (!is_prime_big(last_checked))
            continue;                                       /* not a prime */

        /* check the defining property */
        bool ok = true;
        seq_t pow = (seq_t)2;
        while (pow < last_checked) {
            seq_t rem = last_checked - pow;
            if (rem > 0 && is_prime_big(rem) && rem < pow) {
                ok = false;                                /* decomposition found */
                break;
            }
            /* avoid overflow when doubling pow */
            if (pow > last_checked / 2)
                break;
            pow += pow;   /* next power of two */
        }

        if (ok)                 /* success – store the prime */
            add_to_cache(last_checked);
    }

    return cache_get(n - 1);       /* zero‑based indexing */
}
