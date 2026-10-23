/*
 * A171749  Odd primes of the form (1+n)*(2+2*n)+n*(3+2*n) = 4*n^2+7*n+2.
 * https://oeis.org/A171749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = merge of primes of the form 4*n^2 + 7*n + 2, n odd (OEIS
 *            A171749)
 * Range:     u8 n <= 3, u16 n <= 29, u32 n <= 33, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Lazy‑initialized cache of found primes */
static seq_t *cache     = NULL;   /* array of prime values (truncated to seq_t) */
static size_t cache_cap = 0;      /* capacity of the array               */
static size_t cache_sz  = 0;      /* number of primes stored              */
static idx_t  next_odd  = 1;      /* next odd n to evaluate              */

/* ensure array has at least 'need' capacity */
static void grow_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < need)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence.  Handles calls in any order. */
seq_t a(idx_t n)
{
    if (n == 0)               /* index 0 is outside the OEIS offset */
        return (seq_t)0;

    /* Find the 'n'‑th prime in the sequence. */
    while (cache_sz < n) {
        /* Compute value 4*n^2 + 7*n + 2 using 64‑bit arithmetic. */
        uint64_t nd   = (uint64_t)next_odd;
        uint64_t val  = 4ULL * nd * nd + 7ULL * nd + 2ULL;

        /* Skip values that cannot be represented in seq_t. */
        uint64_t max_seq_t = (uint64_t)~(seq_t)0;
        if (val <= max_seq_t) {
            /* OK to test primality and store truncated value. */
            if (is_prime_big((seq_t)val)) {
                grow_cache(cache_sz + 1);
                cache[cache_sz++] = (seq_t)val;
            }
        }
        /* advance to next odd n */
        next_odd += 2;
    }
    return cache[n - 1];
}
