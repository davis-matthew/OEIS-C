/*
 * A163155  Primes of the form A001654(k) - 2.
 * https://oeis.org/A163155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163155: primes of the form F(k) * F(k+1) – 2,
   where F(k) is the k-th Fibonacci number (F(1)=1, F(2)=1). */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static state: primes cache and Fibonacci generator */
    static seq_t *prime_cache = NULL;
    static size_t prime_cnt  = 0;
    static size_t prime_cap  = 0;

    static seq_t fib_k    = (seq_t)1;  /* F(k)   */
    static seq_t fib_k1   = (seq_t)1;  /* F(k+1) */
    static idx_t  fib_idx = 1;         /* current Fibonacci index (k) */

    /* If the requested value is already cached, return it. */
    if (n > 0 && n <= prime_cnt)
        return prime_cache[n - 1];

    /* Ensure cache capacity. */
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_cache = (seq_t *)malloc(sizeof(seq_t) * prime_cap);
    }

    /* Generate primes until we have at least n of them. */
    while (prime_cnt < n) {
        /* Check if F(k) * F(k+1) - 2 is prime. (All values are seq_t) */
        seq_t prod = fib_k * fib_k1;
        seq_t val  = prod - 2;

        if (is_prime_big(val)) {
            /* Grow cache if necessary. */
            if (prime_cnt >= prime_cap) {
                size_t new_cap = prime_cap * 2;
                seq_t *new_cache =
                    (seq_t *)malloc(sizeof(seq_t) * new_cap);
                memcpy(new_cache, prime_cache,
                       sizeof(seq_t) * prime_cap);
                free(prime_cache);
                prime_cache = new_cache;
                prime_cap   = new_cap;
            }
            prime_cache[prime_cnt++] = val;
        }

        /* Advance Fibonacci numbers. */
        seq_t tmp = fib_k + fib_k1;
        fib_k    = fib_k1;
        fib_k1   = tmp;
        ++fib_idx;
    }

    return prime_cache[n - 1];
}
