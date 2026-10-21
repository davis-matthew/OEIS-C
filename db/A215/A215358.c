/*
 * A215358  Primes congruent to {0, 2, 3, 4} mod 11.
 * https://oeis.org/A215358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 0, 2, 3 or 4 modulo 11. (OEIS
 *            A215358) Offset: 1 (a(1) = 2)
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, residue_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>          /* for size_t, malloc, realloc */

/* External helper: primality test for idx_t (usually uint64_t) */
#include "../utils/is_prime.h"    /* bool is_prime(idx_t x); */
#include "../utils/residue_ok.h"

/* Static cache of found primes */
static seq_t *prime_cache = NULL;   /* dynamically resized array of primes */
static size_t prime_cap   = 0;      /* current allocated capacity      */
static size_t prime_cnt   = 0;      /* number of primes stored         */
static idx_t  next_candidate = 2;   /* next integer to test for primality */

/* Ensure the cache has room for at least 'need' more primes */
static void ensure_capacity(size_t need)
{
    if (prime_cnt + need <= prime_cap)
        return;

    size_t new_cap = prime_cap ? prime_cap : 16;
    while (prime_cnt + need > new_cap)
        new_cap <<= 1;   /* double the capacity */

    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!tmp) /* allocation failure – abort test harness */
        abort();

    prime_cache = tmp;
    prime_cap   = new_cap;
}

/* Append a newly found prime to the cache */
static void append_prime(seq_t p)
{
    ensure_capacity(1);
    prime_cache[prime_cnt++] = p;
}

/* Main sequence function; works in any order of calls */
seq_t a(idx_t n)
{
    if (n < 1)           /* undefined for n < 1 – return 0 */
        return 0;

    /* Generate primes until we have at least n of them */
    while (prime_cnt < n) {
        if (residue_ok(next_candidate) && is_prime(next_candidate))
            append_prime((seq_t)next_candidate);
        ++next_candidate;
    }

    return prime_cache[n - 1];
}
