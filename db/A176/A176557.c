/*
 * A176557  Primes of the form 7*x^2 - 5*y^2, where x and y are successive
 *          natural numbers.
 * https://oeis.org/A176557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth prime of the form 2*n^2 + 10*n - 5 (prime)
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently 7*x^2 - 5*(x-1)^2 for successive naturals x) */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* Cache for previously found terms. */
static seq_t *cache = NULL;      /* array of primes */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static size_t cached_n  = 0;     /* number of primes currently stored */
static idx_t last_x    = 0;      /* last x processed */

/* Ensure the cache can contain at least `need` elements. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1U : 16U;
    while (new_cap < need)
        new_cap <<= 1U;

    /* realloc returns void*, cast explicitly so the code compiles in both C and C++. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for 0, but avoid UB */

    /* Fast return if already cached. */
    if ((size_t)n <= cached_n)
        return cache[(size_t)n - 1];

    /* Generate terms until we have at least n of them. */
    idx_t x = last_x + 1;
    while (cached_n < (size_t)n)
    {
        /* val = 2*x^2 + 10*x - 5 */
        seq_t val = (seq_t)2 * ((seq_t)x * (seq_t)x)
                    + (seq_t)10 * (seq_t)x
                    - (seq_t)5;

        if (is_prime_big(val))
        {
            ensure_cache_capacity(cached_n + 1);
            cache[cached_n] = val;
            ++cached_n;
        }
        last_x = x;
        ++x;
    }

    return cache[(size_t)n - 1];
}
