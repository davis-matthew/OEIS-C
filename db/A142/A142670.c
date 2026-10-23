/*
 * A142670  Primes congruent to 8 mod 57.
 * https://oeis.org/A142670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 174, u32 n <= 105, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A142670: Primes congruent to 8 mod 57
 * a(n) = the n-th prime p such that p ≡ 8 (mod 57), n ≥ 1
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"
#include "../utils/is_prime_big.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;   /* number of terms stored */

    /* static next candidate (always ≡ 8 mod 57) */
    static seq_t next_candidate = 0;
    static bool initialized = false;

    if (!initialized) {
        next_candidate = 8;  /* first number with residue 8 mod 57 */
        initialized = true;
    }

    if (cache_len < (size_t)n) {
        /* Ensure the cache can hold the requested number of terms */
        ensure_capacity(n, &cache_cap, (void **)&cache, sizeof(seq_t));

        /* Generate missing terms */
        while (cache_len < (size_t)n) {
            if (is_prime_big(next_candidate)) {
                cache[cache_len++] = next_candidate;
            }
            next_candidate += 57;  /* keep the residue 8 mod 57 */
        }
    }

    /* Return the requested term (1‑based index) */
    return cache[n - 1];
}
