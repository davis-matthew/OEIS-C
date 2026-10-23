/*
 * A176379  The smallest prime q which stays prime through at least two
 *          iterations of q -> := 2 * q + prime(n+1).
 * https://oeis.org/A176379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   get_prime, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A176379: for a given index n≥1, find the smallest prime q such that
   1) 2q + prime(n+1) is prime,
   2) 4q + 3·prime(n+1) is also prime.
   The function caches computed values so repeated calls are fast. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/get_prime.h"   // uint32_t get_prime(idx_t k)
#include "../utils/next_prime.h"  // uint64_t next_prime(uint64_t start)
#include "../utils/is_prime_big.h"// bool is_prime_big(seq_t n)

static seq_t *cache = NULL;       /* cache[n-1] holds a(n) for n>=1 */
static idx_t cache_len = 0u;      /* number of cached entries */

/* Ensure the cache has room for at least n entries. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;

    seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!new_cache) {
        abort(); /* allocation failure */
    }

    /* Initialize newly allocated space to zero */
    for (idx_t i = cache_len; i < n; ++i)
        new_cache[i] = 0;

    cache = new_cache;
    cache_len = n;
}

/* Compute a(n) if not already cached. */
static void compute_at(idx_t n)
{
    /* prime(n+1) needed for the definition */
    uint32_t p = get_prime(n + 1u);   /* fits in 32 bits */
    seq_t prime_n1 = (seq_t)p;

    seq_t q = 2;  /* start with the smallest prime */
    while (true) {
        seq_t f1 = 2 * q + prime_n1;        /* first iteration */
        if (!is_prime_big(f1)) {
            /* advance q to the next prime */
            uint64_t q_next = next_prime((uint64_t)q + 1u);
            q = (seq_t)q_next;
            continue;
        }

        seq_t f2 = 2 * f1 + prime_n1;        /* second iteration */
        if (is_prime_big(f2)) {
            cache[n - 1] = q;
            return;
        }

        uint64_t q_next = next_prime((uint64_t)q + 1u);
        q = (seq_t)q_next;
    }
}

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0; /* undefined for n < 1; safety value */

    ensure_cache(n);
    if (cache[n - 1] != 0u)
        return cache[n - 1];

    compute_at(n);
    return cache[n - 1];
}
