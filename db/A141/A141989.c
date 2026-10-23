/*
 * A141989  Primes congruent to 13 mod 29.
 * https://oeis.org/A141989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 228, u32 n <= 229, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: primes congruent to 13 mod 29 via incremental search */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n == 0U) return 0;  /* offset is 1, so n==0 is unused */

    /* static cache for found primes */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t count = 0;
    static idx_t  last_candidate = 13U;  /* first candidate of the form */

    /* initialise cache on the first call */
    if (!cache) {
        cap   = 16U;
        cache = seq_alloc(cap);
    }

    /* generate primes until we have at least n items in the cache */
    while (count < (size_t)n) {
        idx_t candidate = last_candidate;
        /* find the next prime of the form 13 mod 29 */
        while (!is_prime_big((seq_t)candidate))
            candidate += 29U;

        /* store the prime */
        cache[count++] = (seq_t)candidate;
        /* prepare for next search */
        last_candidate = candidate + 29U;

        /* grow the cache if necessary */
        if (count == cap) {
            size_t new_cap = cap << 1;
            cache = seq_realloc(cache, cap, new_cap);
            cap = new_cap;
        }
    }

    return cache[n - 1U];
}
