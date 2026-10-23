/*
 * A124129  Primes p for which there are no primes between p and p+sqrt(p).
 * https://oeis.org/A124129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   is_prime_big, sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A124129: primes p for which the next prime is greater than p + ⌊√p⌋. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */
#include "../utils/sqrt_floor.h"    /* sqrt_floor(seq_t) */

/* Cached qualifying primes. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static seq_t prime_start = 1;   /* last prime processed */

/* Ensure the cache array has room for at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) return; /* Allocation failure – unlikely in tests. */
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i) new_arr[i] = cache[i];
        free(cache);
    }
    cache     = new_arr;
    cache_cap = new_cap;
}

/* Return the next prime greater than 'start'. */
static seq_t next_prime_from(seq_t start)
{
    seq_t x = start + 1;
    while (!is_prime_big(x)) ++x;
    return x;
}

/* Return the n-th term of A124129 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, return 0 */

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_len) return cache[n - 1];

    /* Generate terms until we have the n‑th. */
    while (cache_len < (size_t)n) {
        /* Next prime after the last processed prime. */
        seq_t p = next_prime_from(prime_start);

        /* The immediate next prime after p. */
        seq_t q = next_prime_from(p);

        /* Check the condition. */
        seq_t s     = sqrt_floor(p);
        seq_t limit = p + s;
        if (q > limit) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = p;
        }

        /* Prepare for the next iteration. */
        prime_start = p;
    }

    return cache[n - 1];
}
