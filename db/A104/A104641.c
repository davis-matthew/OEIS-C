/*
 * A104641  Numbers n such that n^3 has equal number of odd and even digits.
 * https://oeis.org/A104641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 460, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cube_has_balanced_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the n-th positive integer whose cube has
 * an equal number of odd and even decimal digits.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cube_has_balanced_digits.h"

/* Main sequence function.  We build the list of terms on demand
 * and cache it so that calls in any order are O(n).
 */
seq_t a(idx_t n)
{
    /* cache of already found terms (1‑based indexing stored as 0‑based) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* next natural number to test for the condition */
    static seq_t next_start = 1;

    /* generate terms until we have at least n of them */
    while (len < n) {
        seq_t cand = next_start;
        next_start = cand + 1;

        if (cube_has_balanced_digits(cand)) {
            /* grow cache if necessary */
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();           /* allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = cand;
        }
    }

    /* return the n‑th term (1‑based) */
    return cache[n - 1];
}
