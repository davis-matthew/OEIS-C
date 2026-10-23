/*
 * A184802  Primes of the form floor(k*sqrt(5)).
 * https://oeis.org/A184802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_big, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate k, compute floor(sqrt(5*k*k)) and test prime. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_mul.h"
#include "../utils/floor_sqrt.h"

/* Static cache of primes found so far */
static seq_t *prime_cache = NULL;
static size_t prime_len   = 0;   /* number of stored primes              */
static size_t prime_cap   = 0;   /* current capacity of the cache       */
static idx_t   last_k     = 0;   /* largest k examined so far           */

/* Grow the cache when needed */
static void grow_cache(void)
{
    size_t new_cap = prime_cap ? prime_cap * 2 : 1;
    prime_cache = seq_realloc(prime_cache, prime_cap, new_cap);
    prime_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)               /* out‑of‑range indices are ignored */
        return 0;

    while (prime_len < n) {
        ++last_k;

        /* Compute floor(k * sqrt(5)) as floor(sqrt(5 * k * k)) */
        seq_t kk   = seq_mul((seq_t)last_k, (seq_t)last_k);    /* k*k          */
        seq_t prod = seq_mul((seq_t)5, kk);                    /* 5*k*k        */
        seq_t val  = floor_sqrt(prod);                         /* floor(sqrt)  */

        if (is_prime_big(val)) {
            if (prime_len == prime_cap)
                grow_cache();
            prime_cache[prime_len++] = val;
        }
    }

    return prime_cache[n - 1];
}
