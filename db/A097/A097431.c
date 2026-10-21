/*
 * A097431  Integer part of the hypotenuse of right triangles with consecutive
 *          prime legs.
 * https://oeis.org/A097431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sqrt( prime(n)^2 + prime(n+1)^2 ) )
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Prime cache (0‑based indices).  The functions are thread‑safe only for
 * single‑threaded use (which is the requirement for OEIS tests). */
static uint64_t *prime_cache   = NULL;   /* allocated array of primes          */
static size_t   prime_count    = 0;      /* number of primes currently stored   */
static size_t   prime_capacity = 0;      /* allocated capacity (array size)      */
static uint64_t cand_prime     = 3;      /* next odd number to test for primeness */

/* Ensure prime_cache has enough space for at least `min` primes. */
static void grow_prime_capacity(size_t min)
{
    if (min <= prime_capacity)
        return;
    size_t new_cap = prime_capacity ? prime_capacity << 1 : 32;
    while (new_cap < min)
        new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!tmp) abort();                /* allocation failure – terminate */
    prime_cache   = tmp;
    prime_capacity = new_cap;
}

/* Generate primes until prime_cache holds the prime at 0‑based index `max_idx`. */
static void generate_primes_up_to(size_t max_idx)
{
    if (prime_count == 0) {
        /* initialise with the very first prime */
        grow_prime_capacity(8);
        prime_cache[0] = 2;
        prime_count    = 1;
    }
    while (prime_count <= max_idx) {
        uint64_t p = cand_prime;
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t q = prime_cache[i];
            if (q * q > p) break;
            if ((p % q) == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            grow_prime_capacity(prime_count + 1);
            prime_cache[prime_count++] = p;
        }
        cand_prime += 2;   /* skip even numbers */
    }
}

/* Compute a(n) for n >= 1.  For n == 0 return 0 (offset is 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* behaviour below offset is irrelevant */

    /* Ensure primes for indices n-1 and n are available.  The
     * generation function guarantees a prime at index `max_idx`. */
    generate_primes_up_to((size_t)n);       /* needs primes up to index n */

    /* Retrieve the two consecutive primes (0‑based indices). */
    uint64_t p1 = prime_cache[(size_t)n - 1];   /* prime(n)  */
    uint64_t p2 = prime_cache[(size_t)n];       /* prime(n+1) */

    /* Compute the sum of squares in a type that cannot overflow
     * for all required n (up to the typical test range ~10 000). */
    uint64_t sum_sq = (p1 * p1) + (p2 * p2);

    /* Convert to seq_t and apply the provided integer square‑root. */
    return floor_sqrt((seq_t)sum_sq);
}
