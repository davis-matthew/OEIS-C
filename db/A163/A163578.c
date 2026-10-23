/*
 * A163578  If n is prime, a(n) is next composite greater than a(n-1), else if
 *          n is composite, a(n) is next prime greater than a(n-1) else 2.
 * https://oeis.org/A163578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 6162, u32 n <= 7055, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163578:
 * a(1)=2; for n>1:
 *   if n is prime → a(n) is the smallest composite > a(n‑1)
 *   if n is composite (including 1) → a(n) is the smallest prime > a(n‑1)
 * (n=1 is treated as composite). */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

static seq_t *cache      = NULL;   /* memoised terms, a(1) at cache[0] */
static size_t cache_size = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity */

/* Ensure the cache can hold at least new_cap terms. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t new_cap_real = new_cap > cache_cap * 2 ? new_cap : cache_cap * 2;
    if (new_cap_real == 0) new_cap_real = new_cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap_real * sizeof(seq_t));
    if (!tmp && new_cap_real > 0) exit(EXIT_FAILURE); /* allocation failed */
    cache = tmp;
    cache_cap = new_cap_real;
}

/* Return the first prime number strictly greater than x. */
static seq_t next_prime(seq_t x)
{
    seq_t y = x + 1;
    while (!is_prime_big(y)) ++y;
    return y;
}

/* Return the first composite number strictly greater than x (x>0). */
static seq_t next_composite(seq_t x)
{
    seq_t y = x + 1;
    while (y <= (seq_t)1 || is_prime_big(y)) ++y;
    return y;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined input, harmless return */

    /* initialise cache lazily */
    if (!cache) {
        cache_cap  = 64;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t prev = (i == 1) ? (seq_t)0 : cache[i - 2];
        seq_t val;
        if (is_prime(i)) {            /* n is prime */
            val = next_composite(prev);
        } else {                       /* n is composite (or 1) */
            val = next_prime(prev);
        }
        grow_cache(cache_size + 1);
        cache[cache_size++] = val;
    }

    return cache[n - 1];
}
