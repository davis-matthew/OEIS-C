/*
 * A184803  Numbers k such that floor(k*sqrt(5)) is prime.
 * https://oeis.org/A184803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   ensure_capacity, floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that floor(k*sqrt(5)) is prime
   Computed by iterative search with integer arithmetic:
   floor(k*sqrt(5)) = floor(sqrt(5*k^2)) */

/* The sequence is defined by direct search. The function below
   maintains a cache of the qualifying k values and extends it
   lazily as required. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

/* Cache of found k values */
static seq_t *cache = NULL;          /* array of seq_t */
static size_t cache_cap = 0;         /* current cache capacity */
static idx_t  cached_cnt = 0;        /* number of terms cached */
static idx_t  last_k       = 0;      /* last k processed */

/* Ensure that the cache holds at least n terms. */
static void
ensure_cache(idx_t n)
{
    if (cached_cnt >= n) return;

    /* Extend cache capacity as needed. */
    size_t needed = n;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap < needed) new_cap <<= 1;
        ensure_capacity(new_cap, &cache_cap, (void**)&cache, sizeof(seq_t));
    }

    /* Search for additional qualifying k's. */
    while (cached_cnt < n) {
        ++last_k;                    /* candidate k */
        seq_t k     = (seq_t)last_k;  /* cast to seq_t */
        seq_t prod  = k * k;          /* k^2 */
        prod        = prod * ((seq_t)5);   /* 5 * k^2 */
        seq_t t     = floor_sqrt(prod);    /* floor(k*sqrt(5)) */

        if (is_prime_big(t)) {            /* prime? */
            cache[cached_cnt++] = k;      /* store k (seq_t) */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* below sequence offset, return 0 */
    ensure_cache(n);
    return cache[n - 1];
}
