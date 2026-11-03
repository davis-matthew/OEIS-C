/*
 * A104641  Numbers n such that n^3 has equal number of odd and even digits.
 * https://oeis.org/A104641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 460, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9710 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the n-th positive integer whose cube has
 * an equal number of odd and even decimal digits.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff the cube of m has the same number of odd and even
 * decimal digits.  The function works for any seq_t: it uses only
 * integer arithmetic, comparisons and the bitwise test (d & 1).
 */
static inline bool cube_has_balanced_digits(seq_t m)
{
    seq_t cube = m * m * m;
    seq_t even = 0, odd = 0;

    while (cube > 0) {
        seq_t d = cube % 10;
        /* (d & 1) is 0 for even digits, 1 for odd digits */
        if ((d & 1) == 0)
            even++;
        else
            odd++;
        cube /= 10;
    }
    return even == odd;
}

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
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
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
