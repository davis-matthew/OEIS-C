/*
 * A045256  Numbers whose base-5 representation contains exactly two 1's and
 *          one 4.
 * https://oeis.org/A045256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_two_ones_one_four
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045256: Numbers whose base-5 representation contains exactly two 1's and one 4.
 * Computed by brute‑force incremental search. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_two_ones_one_four.h"

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n = 0 */

    static seq_t *cache = NULL;         /* array holding discovered terms */
    static size_t len  = 0;             /* current number of cached terms   */
    static size_t cap  = 0;             /* allocated capacity               */
    static seq_t last_checked = 0;      /* largest number previously tested */

    /* Ensure the cache can hold at least n terms. */
    if (len < (size_t)n) {
        if (cap < (size_t)n) {
            size_t new_cap = (cap == 0 ? 16 : cap * 2);
            if (new_cap < (size_t)n) new_cap = (size_t)n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap   = new_cap;
        }

        /* Search forward until we have found n terms. */
        seq_t cand = last_checked + 1;
        while (len < (size_t)n) {
            if (has_two_ones_one_four(cand)) {
                cache[len++] = cand;
            }
            ++cand;
        }
        last_checked = cand - 1;
    }

    return cache[n - 1];
}
