/*
 * A120533  Primes having a prime number of digits.
 * https://oeis.org/A120533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 5477, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 8365 (2026-11-03)
 * Helpers:   is_prime_seq, mem, next_prime, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes having a prime number of decimal digits. */
/* Each call expands an internal cache using next_prime() and digit_count(). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Utilities (available in the build) */
#include <math.h>
#include "../utils/is_prime_seq.h"
#include "../utils/next_prime.h"
#include "../utils/num_digits0.h"
#include "../utils/mem.h"

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

    seq_t *tmp = mem_new(seq_t, (size_t)new_cap);
    if (!tmp)
        abort();  /* Allocation failed */

    /* Copy existing elements (copy constructor will be invoked in C++). */
    for (idx_t i = 0; i < cache_size; ++i)
        tmp[i] = prime_cache[i];

    mem_free(prime_cache);
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
        uint64_t digits = num_digits0(prime);
        if (is_prime_seq((seq_t)digits)) {
            /* Store in cache. */
            prime_cache[cache_size++] = prime;
        }
    }

    return prime_cache[n - 1U];
}
