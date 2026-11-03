/*
 * A104147  Number of cubes <= n-th prime.
 * https://oeis.org/A104147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ⌊cuberoot(p_n)⌋ + 1, where p_n is the n‑th prime
 *            (0‑based)
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 114 terms through n = 886 (2026-11-03)
 * Helpers:   icbrt_seq, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/icbrt_seq.h"
#include "../utils/is_prime_u64.h"

static uint64_t *prime_cache = NULL;   /* cached prime values (uint64_t)           */
static idx_t    cache_size  = 0;       /* number of stored primes                 */

/* Ensure we have at least (n+1) primes cached */
static bool grow_cache(idx_t n)
{
    if (n < cache_size) return true;          /* already have enough */

    idx_t new_size = n + 1;                   /* primes needed */
    uint64_t *new_arr = (uint64_t *)malloc(new_size * sizeof(uint64_t));
    if (new_arr == NULL) return false;        /* allocation failed */

    /* Copy existing primes if any */
    for (idx_t i = 0; i < cache_size; ++i)
        new_arr[i] = prime_cache[i];

    /* Generate missing primes */
    uint64_t candidate = (cache_size == 0) ? 2 : prime_cache[cache_size - 1] + 1;
    idx_t    count     = cache_size;

    while (count < new_size) {
        if (is_prime_u64(candidate)) {
            new_arr[count] = candidate;
            ++count;
        }
        ++candidate;
    }

    free(prime_cache);
    prime_cache = new_arr;
    cache_size  = new_size;
    return true;
}

seq_t a(idx_t n)
{
    if (!grow_cache(n)) return 0;             /* allocation error case */

    uint64_t p = prime_cache[n];              /* n-th prime (uint64_t) */
    seq_t  k = icbrt_seq(p);           /* floor(cuberoot(p))   */
    return k + 1;                             /* include the cube 0^3 */
}
