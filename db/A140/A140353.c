/*
 * A140353  a(n) = prime(n) + 9.
 * https://oeis.org/A140353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) + 9 (n ≥ 1)
 * Range:     u8 n <= 53, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static cache holding the first primes */
static idx_t *prime_cache = NULL;   /* array of primes (uint64_t) */
static size_t prime_cnt   = 0;      /* number of primes stored */
static size_t prime_cap   = 0;      /* current capacity of prime_cache */
static idx_t  next_cand   = 3;      /* next odd candidate to test */

/* Ensure the cache contains at least n primes (n ≥ 1). */
static void
ensure_primes(idx_t n)
{
    if (prime_cnt >= n)
        return;

    /* Initialise cache on first call. */
    if (prime_cache == NULL) {
        prime_cap  = 16;
        prime_cache = (idx_t *)malloc(prime_cap * sizeof(idx_t));
        if (!prime_cache) abort();
        prime_cache[0] = 2;          /* first prime */
        prime_cnt   = 1;
        next_cand   = 3;
    }

    /* Generate primes until we have at least n of them. */
    while (prime_cnt < n) {
        idx_t cand = next_cand;
        next_cand += 2;                 /* skip evens */

        /* Test cand for primality. */
        int is_prime = 1;                /* assume prime */
        for (size_t j = 0; j < prime_cnt; ++j) {
            idx_t p = prime_cache[j];
            if (p > cand / p)           /* p*p > cand : no further factors possible */
                break;
            if (cand % p == 0) {        /* composite */
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {                  /* store prime */
            if (prime_cnt == prime_cap) {  /* grow cache */
                prime_cap <<= 1;
                idx_t *tmp = (idx_t *)realloc(prime_cache,
                                               prime_cap * sizeof(idx_t));
                if (!tmp) abort();
                prime_cache = tmp;
            }
            prime_cache[prime_cnt++] = cand;
        }
    }
}

/* OEIS sequence a(n) = prime(n) + 9 for n ≥ 1. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1 */

    ensure_primes(n);
    return (seq_t)prime_cache[n - 1] + (seq_t)9;
}
