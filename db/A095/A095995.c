/*
 * A095995  Primes of the form 100n - 1.
 * https://oeis.org/A095995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 120, u32 n <= 121, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computation of OEIS A095995: primes of the form 100 m − 1.
 * The sequence is enumerated by increasing m starting at 2.
 * a(1) = 199 = 100 × 2 − 1, a(2) = 499 = 100 × 5 − 1, …
 * The function returns the n‑th such prime (n ≥ 1); for n = 0 it returns 0.
 * Internally, it caches the primes found so far and continues the search
 * on subsequent calls.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/*
 * seq_t and idx_t are supplied by the build system.
 * seq_t is an unsigned integer type (possibly arbitrary precision).
 * idx_t is uint64_t.
 */

seq_t a(idx_t n)
{
    /* Undefined for offset zero; return zero for safety. */
    if (n == 0)
        return 0;

    /* Static cache of found primes. */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static size_t  len   = 0;
    static idx_t   m     = 2;      /* last m examined */

    /* Extend cache until we have at least n primes. */
    while (len < n) {
        /* Compute candidate 100*m − 1 using 64‑bit arithmetic to avoid
           overflow in the intermediate product. */
        uint64_t cand64 = (uint64_t)m * 100ULL - 1ULL;
        seq_t   cand   = (seq_t)cand64;

        if (is_prime_big(cand)) {
            /* Grow the cache if necessary. */
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 8 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) {
                    /* Allocation failed; abort with a defined behaviour. */
                    /* In a real implementation we might handle this differently,
                       but for the purposes of the sequence generator we can
                       simply exit. */
                    abort();
                }
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = cand;
        }
        ++m;
    }

    return cache[(size_t)(n - 1)];
}
