/*
 * A091858  a(n) = n! mod prime(n).
 * https://oeis.org/A091858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 6607, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A091858
   a(n) = n!  mod  prime(n)   (offset 1)
   (prime(n) = the n-th prime, 1‑based) */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/estimate_prime_upper_bound.h"

/* Prime table (1‑based indexing, prime[0] = prime(1)) */
static size_t prime_count = 0;       /* number of primes currently stored */
static size_t prime_capacity = 0;    /* allocated size of prime_table */
static uint64_t *prime_table = NULL;

/* Build (or enlarge) the prime table to contain at least the first n primes. */
static void build_primes_at_least(size_t n) {
    if (prime_count >= n) return;        /* already have enough */

    /* Compute sieve limit; increase a bit to reduce re‑sieves. */
    size_t limit = estimate_prime_upper_bound(n);
    if (limit < 2) limit = 2;

    /* Allocate sieve array. */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return;               /* memory failure – unlikely */

    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes up to limit. */
    size_t needed = n;
    if (prime_capacity < needed) {
        /* Allocate larger array (or create new). */
        uint64_t *new_tbl = (uint64_t *)malloc(needed * sizeof(uint64_t));
        if (!new_tbl) {
            free(is_prime);
            return;                      /* memory failure */
        }
        if (prime_table) free(prime_table);
        prime_table = new_tbl;
        prime_capacity = needed;
    }

    /* Fill prime_table. */
    size_t idx = 0;
    for (size_t i = 2; i <= limit && idx < needed; ++i) {
        if (is_prime[i]) {
            prime_table[idx++] = (uint64_t)i;
        }
    }

    prime_count = idx;

    free(is_prime);
}

/* ------------------------------------------------------------ */
/*  Main sequence function                                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined; guard for safety */

    /* Ensure we have prime(n). */
    build_primes_at_least((size_t)n);

    uint64_t p = prime_table[(size_t)n - 1];  /* 1‑based indexing */

    /* Compute n! modulo p.  Use unsigned __int128 in the loop
       to avoid overflow of the intermediate product. */
    unsigned __int128 prod = 1;
    for (idx_t k = 2; k <= n; ++k) {
        prod = (prod * (unsigned __int128)k) % (unsigned __int128)p;
    }

    return (seq_t)prod;                     /* cast to sequence type */
}
