/*
 * A097785  Counterexamples to the conjecture that an even, prime-indexed
 *          triangular plus 1 equals a prime or that an odd, prime-indexed
 *          triangular minus 2 equals a prime.
 * https://oeis.org/A097785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 30, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   isPrimeGeneric, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = nth counterexample to the conjecture that for every prime p,
    if Tp = p*(p+1)/2 is even then Tp + 1 is prime, and if Tp is odd
    then Tp – 2 is prime.  The function searches primes in order,
    computes the corresponding value, and stores the composites in a cache. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/is_prime.h"

/* Helper to grow a dynamic array */
static void grow_if_needed(void **ptr, size_t *cap, size_t needed, size_t elem_size)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0 ? 16 : (*cap << 1));
        if (new_cap < needed) new_cap = needed;
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap = new_cap;
    }
}

/*  The sequence cache */
static size_t cache_len    = 0;   /* number of terms computed so far */
static size_t cache_cap    = 0;   /* capacity of the cache array          */
static seq_t *cache        = NULL;/* array holding the terms               */

/*  State used to generate the terms lazily */
static uint64_t next_candidate = 2;  /* next integer to test for primality   */

seq_t a(idx_t n)
{
    /* Generate terms on demand */
    while (cache_len < (size_t)n) {
        /* Skip non‑primes */
        while (!is_prime(next_candidate)) ++next_candidate;

        /* Compute the triangular number Tp */
        uint64_t tri = (uint64_t)next_candidate * (next_candidate + 1) / 2;

        /* Determine the value that should be prime by the conjecture */
        seq_t val;
        if ((tri & 1U) == 0)           /* Tp even  -> Tp + 1 */
            val = (seq_t)(tri + 1);
        else                           /* Tp odd   -> Tp - 2 */
            val = (seq_t)(tri - 2);

        /* Record the term if it is composite (and >1 to avoid the trivial case p = 2) */
        if (val > (seq_t)1 && !isPrimeGeneric(val)) {
            grow_if_needed((void **)&cache, &cache_cap, cache_len + 1, sizeof(seq_t));
            cache[cache_len++] = val;
        }

        ++next_candidate;   /* advance to the next candidate for prime testing */
    }

    return cache[n - 1];
}
