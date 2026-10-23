/*
 * A030090  Primes p such that digits of p do not appear in p^2 or p^3
 *          (probably finite).
 * https://oeis.org/A030090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, hard
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   digit_mask_uint128, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A030090 – primes p such that none of the decimal digits
 * of p appear in p² or in p³.
 *
 * The function builds the sequence on demand.  It keeps a static
 * cache of the first k terms and extends it when a larger index
 * is requested.  Primes are tested sequentially with the supplied
 * is_prime() routine.  For each candidate prime p we mask the
 * decimal digits of p, p² and p³ using a 128‑bit integer to avoid
 * overflow of any supported seq_t.  The term is stored in the cache
 * only if the p digit set is disjoint from those of p² and p³.
 *
 * The function has external linkage as required:
 *     seq_t a(idx_t n)
 *
 * It works with any seq_t and idx_t definition provided by the
 * build system and is compliant with C11 and C++17.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/digit_mask_uint128.h"

/* The sequence function */
seq_t a(idx_t n)
{
    /* Static cache of terms.  cache_len is the number of terms stored. */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Initialise cache capacity if first call. */
    if (cache_cap == 0) {
        cache_cap = 16;                 /* start with a small capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Find the next prime beyond the last tested candidate. */
        static uint64_t candidate = 1;
        do {
            ++candidate;
        } while (!is_prime((idx_t)candidate));

        /* Candidate is confirmed prime.  Test the digit condition. */
        unsigned p_mask   = digit_mask_uint128((__uint128_t)candidate);
        __uint128_t sq    = (__uint128_t)candidate * candidate;
        unsigned sq_mask  = digit_mask_uint128(sq);
        if (p_mask & sq_mask)
            continue;                           /* digit in p² */

        __uint128_t cube   = sq * candidate;
        unsigned cube_mask = digit_mask_uint128(cube);
        if (p_mask & cube_mask)
            continue;                           /* digit in p³ */

        /* Store the prime. */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = (seq_t)candidate;
    }

    /* Return the n‑th term (1‑based index). */
    return cache[n - 1];
}
