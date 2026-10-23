/*
 * A142454  Primes congruent to 47 mod 49.
 * https://oeis.org/A142454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 141, u32 n <= 74, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142454: primes congruent to 47 mod 49 */
/* Computation: iterate candidates p = 47 + 49*k, test primality by is_prime_big */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;        /* array of found primes */
static size_t cap = 0;             /* allocated capacity */
static idx_t  max_n = 0;           /* number of primes stored */
static seq_t  next_candidate = 47; /* next number to test, congruent to 47 mod 49 */

/* grow cache to accommodate at least need primes */
static void ensure_cache(idx_t n)
{
    const seq_t inc = (seq_t)49;
    while (max_n < n)
    {
        if (is_prime_big(next_candidate))
        {
            /* extend cache if necessary */
            if (max_n >= cap)
            {
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();             /* out‑of‑memory */
                cache = tmp;
                cap   = newcap;
            }
            cache[max_n] = next_candidate;
            ++max_n;
        }
        next_candidate += inc;
    }
}

/* external function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined only for n >= 1 */
    ensure_cache(n);
    return cache[n - 1];
}
