/*
 * A090187  Primes of the form 11*n+2.
 * https://oeis.org/A090187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 2 modulo 11 (p = 11*k + 2)
 * Range:     u8 n <= 7, u16 n <= 663, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Simple trial‑division primality test for an arbitrary unsigned integer type.
   It works for seq_t up to 128 bits and safely handles small values. */
static bool is_prime_seq(seq_t val)
{
    if (val < 2u) return false;          /* 0,1 are not prime */
    if (val % 2u == 0u) return val == 2u;
    if (val % 3u == 0u) return val == 3u;

    /* Check divisibility by 6k±1 up to sqrt(val).
       Use division instead of multiplication to avoid overflow. */
    seq_t i = 5u;
    while (i <= val / i) {
        if (val % i == 0u) return false;
        if (val % (i + 2u) == 0u) return false;
        i += 6u;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1; n==0 is undefined; return 0 */

    /* Static cache for already found primes of the form 11*k+2. */
    static seq_t *cache      = NULL;
    static idx_t  cache_len  = 0;
    static idx_t  cache_cap  = 0;
    static idx_t  next_k     = 0;   /* next k to test, starting from 0 */

    /* Largest value representable by seq_t. */
    const seq_t seq_max = (seq_t)-1;

    /* Initialize cache lazily. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate primes until we have at least n of them. */
    while (cache_len < n) {
        /* Candidate number: 11*next_k + 2.  Use idx_t arithmetic to avoid
           overflow in the multiplication; next_k stays reasonably small. */
        idx_t cand64 = 11ull * next_k + 2ull;

        /* If the candidate cannot fit into seq_t we stop generating. */
        if (cand64 > (idx_t)seq_max) {
            break; /* Further primes would overflow seq_t and are not needed. */
        }

        seq_t candidate = (seq_t)cand64;

        if (is_prime_seq(candidate)) {
            /* Grow cache capacity if necessary. */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = candidate;
        }
        ++next_k;
    }

    /* Return the n‑th prime (1‑based index). */
    return cache[n - 1];
}
