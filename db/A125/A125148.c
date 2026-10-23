/*
 * A125148  a(n) = smallest prime p = z + x, where x is the n-th odd composite
 *          number not divisible by 5 and z is a multiple of 10.
 * https://oeis.org/A125148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest prime p such that p ≡ x (mod 10), where x is the n-th odd composite
   number not divisible by 5.  Computed by sequential search, caching computed values. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* static cache variables */
    static idx_t max_cached = 0;           /* maximum n cached so far */
    static size_t cache_cap = 0;           /* storage capacity */
    static uint64_t *odd_comp = NULL;      /* nth odd composite not divisible by 5 */
    static uint64_t *prime_val = NULL;     /* corresponding a(n) value */
    static uint64_t next_candidate = 9;    /* next odd composite to test */

    /* if we already have the required term, return it */
    if (n <= max_cached) {
        return (seq_t)prime_val[n];
    }

    /* extend cache until we reach n */
    while (max_cached < n) {
        /* find next odd composite not divisible by 5 */
        uint64_t cand = next_candidate;
        while (1) {
            if ((cand % 5U) != 0U && !is_prime_small(cand)) {
                /* cand is odd by construction, composite and not multiple of 5 */
                break;
            }
            cand += 2U;   /* advance to next odd number */
        }

        /* compute a(n): smallest prime p > cand with p ≡ cand (mod 10) */
        uint64_t p = cand + 10U;
        while (!is_prime_small(p)) {
            p += 10U;
        }

        /* grow cache if necessary */
        if ((size_t)(max_cached + 1) > cache_cap) {
            size_t new_cap = (cache_cap == 0 ? 16 : cache_cap * 2);
            uint64_t *tmp;
            tmp = (uint64_t *)realloc(odd_comp, new_cap * sizeof(uint64_t));
            if (!tmp) return (seq_t)0;   /* allocation failure; return 0 */
            odd_comp = tmp;
            tmp = (uint64_t *)realloc(prime_val, new_cap * sizeof(uint64_t));
            if (!tmp) return (seq_t)0;
            prime_val = tmp;
            cache_cap = new_cap;
        }

        /* store new values */
        odd_comp[max_cached + 1] = cand;
        prime_val[max_cached + 1] = p;
        ++max_cached;

        /* prepare next candidate */
        next_candidate = cand + 2U;
    }

    return (seq_t)prime_val[n];
}
