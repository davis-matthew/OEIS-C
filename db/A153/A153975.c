/*
 * A153975  Values of n such that n^2-3 and n^2+3 are both prime.
 * https://oeis.org/A153975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth integer n such that n²-3 and n²+3 are prime (OEIS
 *            A153975)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 828, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n) {
    /* static cache of found sequence values */
    static seq_t *cache = NULL;      /* array of seq_t values */
    static size_t cache_len = 0;     /* number of values stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t next_candidate = 1; /* next n to test */

    /* If we already have enough values, return the requested one */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Generate new terms until we reach the desired index */
    while (cache_len < n) {
        for (;;) {
            /* Increment candidate */
            next_candidate++;
            seq_t c = (seq_t)next_candidate;
            seq_t sq = c * c;
            seq_t sq_minus = sq - (seq_t)3;
            seq_t sq_plus  = sq + (seq_t)3;

            /* test primality of both n²-3 and n²+3 */
            if (isPrimeGeneric(sq_minus) && isPrimeGeneric(sq_plus)) {
                /* append found value to cache */
                append_candidate(&cache, &cache_len, &cache_cap, c);
                break; /* move on to next term */
            }
        }
    }

    return cache[n - 1];
}
