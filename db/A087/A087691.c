/*
 * A087691  Squares of primes of the form 4*k+3.
 * https://oeis.org/A087691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-th prime of the form 4k+3)^2 (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 29, u32 n <= 3284, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, safe_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/safe_realloc.h"

/* Static cache of primes that are ≡ 3 (mod 4) */
static seq_t *primes = NULL;
static size_t primes_len = 0; /* number of primes stored          */
static size_t primes_cap = 0; /* allocated capacity of the array  */
static idx_t   next_candidate = 3; /* next 4k+3 integer to test */

seq_t a(idx_t n)
{
    /* If n <= 0 we return 0 – the OEIS offset starts at 1. */
    if (n == 0)
        return 0;

    /* Generate primes until we have at least n of them. */
    while (primes_len < (size_t)n)
    {
        if (is_prime((idx_t)next_candidate))
        {
            /* Ensure capacity. */
            if (primes_len + 1 > primes_cap)
            {
                size_t new_cap = (primes_cap == 0) ? 16 : primes_cap * 2;
                primes = (seq_t *)safe_realloc(primes, new_cap * sizeof(seq_t));
                primes_cap = new_cap;
            }
            primes[primes_len++] = (seq_t)next_candidate;
        }
        /* Keep the candidate of the form 4k+3. */
        next_candidate += 4;
    }

    /* Return the square of the n-th prime (1‑based). */
    seq_t p = primes[n - 1];
    return p * p;
}
