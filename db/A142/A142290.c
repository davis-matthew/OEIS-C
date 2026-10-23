/*
 * A142290  Primes congruent to 41 mod 43.
 * https://oeis.org/A142290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 152, u32 n <= 49, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 41 (mod 43).
   The nth term is the nth prime p such that p % 43 == 41.  The
   implementation generates candidates of the form 41 + 43*k
   and tests each with is_prime_big().  A static cache of found primes
   makes repeated calls fast and independent of call order. */

#include <stddef.h>               /* size_t */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t) */
#include "../utils/seq_alloc.h"   /* seq_t *seq_alloc(size_t) */

static seq_t *cache      = NULL; /* cache of found primes */
static const size_t CACHE_CAPACITY = 16384; /* large enough for n≈10000 */
static idx_t     primes_found = 0;          /* number of primes stored */
static idx_t     next_k        = 0;          /* next candidate index (k) */

/* Return the n-th prime ≡ 41 (mod 43).  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                    /* undefined for offset 0 */

    /* Allocate cache once. */
    if (!cache)
        cache = seq_alloc(CACHE_CAPACITY);

    /* Generate candidates until we have at least n primes. */
    while (primes_found < n) {
        seq_t cand = (seq_t)41 + (seq_t)43 * (seq_t)next_k;
        if (is_prime_big(cand)) {
            if (primes_found < CACHE_CAPACITY)
                cache[primes_found] = cand;
            primes_found++;
        }
        next_k++;
    }

    return cache[n - 1];
}
