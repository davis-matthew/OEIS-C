/*
 * A144486  Triangular numbers n*(n+1)/2 with n and n+1 composite, where
 *          number of prime factors in n = number of prime factors in n+1.
 *          (Prime factors are counted with multiplicity.)
 * https://oeis.org/A144486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2, where n and n+1 are composite and have the
 *            same number of prime factors counted with multiplicity
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   count_prime_factors, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    /* Static state allows calls in any order. */
    static idx_t *candidates = NULL;   /* found n values           */
    static size_t cap          = 0;    /* allocated capacity       */
    static size_t cnt          = 0;    /* number of stored n's     */
    static idx_t  start        = 2;    /* next integer to test     */

    /* Ensure space for more candidates. */
    if (cnt >= cap) {
        size_t new_cap = (cap == 0) ? 16 : cap * 2;
        idx_t *tmp = (idx_t *)realloc(candidates, new_cap * sizeof(idx_t));
        if (!tmp) exit(EXIT_FAILURE);                /* out of memory */
        candidates = tmp;
        cap = new_cap;
    }

    /* Find enough qualifying n's. */
    while (cnt < (size_t)n) {
        /* Skip integers smaller than 2 (cannot be composite). */
        while (start <= 1) start++;

        /* Both n and n+1 must be composite. */
        if (is_prime(start) || is_prime(start + 1)) {
            start++;
            continue;
        }

        /* Count prime factors with multiplicity. */
        seq_t cf1 = count_prime_factors((seq_t)start);
        seq_t cf2 = count_prime_factors((seq_t)(start + 1));

        if (cf1 == cf2) {
            /* Store the qualifying n. */
            candidates[cnt++] = start;
        }
        start++;
    }

    /* Retrieve the requested n. */
    idx_t val = candidates[n - 1];

    /* Compute the triangular number in seq_t. */
    seq_t result = ((seq_t)val) * (((seq_t)val) + 1) / 2;
    return result;
}
