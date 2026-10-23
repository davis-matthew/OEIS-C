/*
 * A120533  Primes having a prime number of digits.
 * https://oeis.org/A120533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 5477, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count, is_prime_small, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes having a prime number of decimal digits. */
/* Each call expands an internal cache using next_prime() and digit_count(). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Utilities (available in the build) */
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t) */
#include "../utils/digit_count.h"  /* uint64_t digit_count(uint64_t) */
#include "../utils/is_prime_small.h" /* bool is_prime_small(seq_t) */

/* --- Internal cache --------------------------------------------------- */
static seq_t *prime_cache = NULL;  /* array of primes */
static idx_t  cache_size = 0;      /* number of stored primes */
static idx_t  cache_cap  = 0;      /* allocated capacity */
static uint64_t last_checked = 2;  /* last prime examined */

/* Grow the cache to at least min_cap entries, reallocating memory safely. */
static void grow_cache(idx_t min_cap)
{
    if (cache_cap >= min_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < min_cap)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
    if (!tmp)
        abort();  /* Allocation failed */

    /* Copy existing elements (copy constructor will be invoked in C++). */
    for (idx_t i = 0; i < cache_size; ++i)
        tmp[i] = prime_cache[i];

    free(prime_cache);
    prime_cache = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)  /* behaviour below the offset is irrelevant */
        return 0;

    /* Return cached value if already known. */
    if (n <= cache_size)
        return prime_cache[n - 1U];

    /* Ensure the cache has enough capacity. */
    grow_cache(n);

    /* Generate primes until we have at least n terms. */
    while (cache_size < n) {
        /* Find next prime > last_checked. */
        uint64_t prime = next_prime(last_checked);
        last_checked = prime;

        /* Count decimal digits and test if that count is prime. */
        uint64_t digits = digit_count(prime);
        if (is_prime_small((seq_t)digits)) {
            /* Store in cache. */
            prime_cache[cache_size++] = prime;
        }
    }

    return prime_cache[n - 1U];
}
