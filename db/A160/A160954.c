/*
 * A160954  a(n+1) is the smallest prime formed by appending a single digit to
 *          a(n); a(1)=18.
 * https://oeis.org/A160954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A160954
 *   a(1) = 18
 *   a(n+1) = smallest prime that can be formed by appending a single decimal digit
 *            to a(n).  If no such prime exists the sequence terminates.
 *
 * The routine memoises the sequence so that calls in any order are answered
 * correctly, provided that the requested value fits in seq_t.  For the 64‑bit
 * build only terms that fit in 64 bits are required.  For the arbitrary‑
 * precision build all terms are produced exactly.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* undefined for n < 1 */

    static seq_t *cache = NULL;
    static size_t size   = 0;   /* number of cached terms */
    static size_t cap    = 0;   /* allocated capacity */
    static bool   finished = false; /* true once no successor exists */

    /* initialise with first term if necessary */
    if (size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */
        cache[0] = (seq_t)18;
        size   = 1;
        cap    = 1;
    }

    /* extend cache until it contains at least n terms or the sequence ends */
    while (!finished && size < (size_t)n) {
        /* grow buffer if needed */
        if (size == cap) {
            size_t new_cap = (cap == 0) ? 8 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;       /* allocation failure */
            cache = tmp;
            cap   = new_cap;
        }

        seq_t prev = cache[size - 1];
        seq_t cand = 0;
        bool found = false;

        for (int d = 0; d <= 9; ++d) {
            cand = prev * (seq_t)10 + (seq_t)d;
            if (is_prime_big(cand)) {
                found = true;
                break;
            }
        }

        if (!found) {
            finished = true;          /* no further term possible */
            break;
        }

        cache[size++] = cand;           /* store the new term */
    }

    /* return the requested term if available */
    if (size >= (size_t)n) return cache[(size_t)n - 1];
    return 0;                          /* sequence ended before n */
}
