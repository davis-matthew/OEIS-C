/*
 * A128292  Primes not in A126769.
 * https://oeis.org/A128292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime that cannot be written as k^4 + s (k>1,
 *            s≥1) where k^2+s is prime. The implementation searches primes in
 *            ascending order and stores the qualifying ones in a lazy‑built
 *            cache. Any call to a(n) retrieves the precomputed value
 * Range:     u8 n <= 19, u16 n <= 175, u32 n <= 182, u64 n <= 231,
 *            u128 n <= 231, big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* ----- cache for the sequence ----- */
static seq_t *cache      = NULL;   /* array of accepted primes          */
static size_t cache_len  = 0;      /* number of primes stored           */
static size_t cache_cap  = 0;      /* capacity of the array             */

/* ----- helper to grow the cache ----- */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

static void add_prime(seq_t p)
{
    grow_cache(cache_len + 1);
    cache[cache_len++] = p;
}

/* ----- test whether p violates the A126769 condition ----- */
static bool violates_condition(seq_t p)
{
    /* k starts at 2 and increases until k^4 >= p */
    for (idx_t k = 2; ; ++k)
    {
        seq_t k_sq = (seq_t)k * k;
        if (k_sq == 0) break;                /* overflow */
        seq_t k_quad = k_sq * k_sq;
        if (k_quad == 0) break;              /* overflow */
        if (k_quad >= p) break;              /* s would be < 1 */

        /* candidate = k^2 + (p - k^4) = p - k^4 + k^2  */
        seq_t candidate = p - k_quad + k_sq;
        if (is_prime_big(candidate))
            return true;   /* forbidden representation found */
    }
    return false;
}

/* ----- main function required by the framework ----- */
seq_t a(idx_t n)
{
    /* Build the cache lazily; support arbitrary call order. */
    while (cache_len < (size_t)n)
    {
        static idx_t current = 2;
        while (!is_prime_big((seq_t)current)) ++current;   /* next prime */

        seq_t p = (seq_t)current;
        ++current;   /* advance for subsequent calls */

        if (!violates_condition(p))
            add_prime(p);
    }
    return cache[n - 1];   /* a(1) is the first element */
}
