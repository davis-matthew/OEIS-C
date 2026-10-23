/*
 * A158038  Difference between n-th prime and next cube.
 * https://oeis.org/A158038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A158038: a(n) = (ceil(∛pₙ))³ – pₙ,
   where pₙ is the n‑th prime (n ≥ 1). */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/int_cube_root.h"

/* Cached primes for repeated calls */
static uint64_t *prime_cache = NULL;
static size_t prime_cache_count = 0;   /* number of primes currently cached */

/* Ensure that at least 'n' primes are in the cache (n ≥ 1).
   For n small a simple upper bound works; for larger values the
   limit is doubled until enough primes are found. */
static void ensure_primes(idx_t n)
{
    if (prime_cache_count >= (size_t)n)
        return;

    /* Start with a modest bound.  For n ≤ 10000 the 10000th prime is 104729;
       20×n comfortably exceeds this. */
    idx_t limit = (idx_t)(n * 20);
    if (limit < 100)
        limit = 100;

    uint64_t *tmp = NULL;
    size_t cnt = 0;
    build_primes_upto(limit, &tmp, &cnt);

    /* If the bound was too low, double it until enough primes are found */
    while (cnt < (size_t)n) {
        free(tmp);
        limit *= 2;
        build_primes_upto(limit, &tmp, &cnt);
    }

    /* Replace old cache with the new one */
    free(prime_cache);
    prime_cache = tmp;
    prime_cache_count = cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for n < 1 */

    ensure_primes(n);
    uint64_t p = prime_cache[(size_t)(n - 1)];  /* nth prime, 0‑based index */

    /* smallest integer k such that k³ > p */
    idx_t k = (idx_t)int_cube_root(p) + 1;

    seq_t cube = (seq_t)k * (seq_t)k * (seq_t)k;
    return cube - (seq_t)p;
}
