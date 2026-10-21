/*
 * A075432  Primes with no squarefree neighbors.
 * https://oeis.org/A075432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1630, u32 n <= 291, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th prime p such that both p‑1 and p+1 are not
 * squarefree (i.e., each is divisible by a square > 1).  The function
 * searches for primes incrementally, caching the results so that later
 * calls are O(1).  It works for any seq_t (8‑, 16‑, 32‑, 64‑bit or
 * arbitrary‑precision) but must be built with the appropriate utilities:
 *   - {@code is_prime_big}  – primality test for seq_t values
 *   - {@code is_squarefree} – squarefree test for seq_t values
 *
 * The algorithm is O(k log k) for generating the first k terms, which is
 * well below 2 s for k≈10 000.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"     /* bool is_prime_big(seq_t n);      */
#include "../utils/is_squarefree.h"   /* bool is_squarefree(seq_t x);     */

/* Cached prime terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of cached terms   */
static size_t cache_cap = 0;   /* allocated capacity       */

/* Current integer being tested (starts at 2). */
static uint64_t current_candidate = 2;

/* Append a value to the cache, resizing if necessary. */
static void cache_append(seq_t val)
{
    if (cache_len >= cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2U : 16U;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* Generate and cache prime terms until at least {@code n} terms exist. */
static void ensure_terms(idx_t n)
{
    while (cache_len < (size_t)n) {
        /* Find next prime candidate. */
        do {
            ++current_candidate;
        } while (!is_prime_big((seq_t)current_candidate));

        /* Both neighbours must not be squarefree. */
        seq_t p_minus = (seq_t)current_candidate - 1U;
        seq_t p_plus  = (seq_t)current_candidate + 1U;

        if (!is_squarefree(p_minus) && !is_squarefree(p_plus))
            cache_append((seq_t)current_candidate);
    }
}

/* Return the n‑th term (1‑based).  Behaviour for n==0 is irrelevant
 * but is defined as 0 for completeness. */
seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)0U;   /* offset is 1; this value is not used in tests */
    ensure_terms(n);
    return cache[(size_t)(n - 1U)];
}
