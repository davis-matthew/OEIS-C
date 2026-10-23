/*
 * A039539  Numbers whose base-12 representation has the same nonzero number
 *          of 5's and 7's.
 * https://oeis.org/A039539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_equal_5_7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑12 representation has the same nonzero
   number of 5's and 7's (OEIS A039539) */
#include <stdlib.h>
#include "../utils/has_equal_5_7.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n < 1 */

    static seq_t *terms = NULL;
    static size_t sz   = 0;
    static size_t cap  = 0;
    static seq_t next_value = (seq_t)1;

    size_t need = (size_t)n;

    if (need > sz) {
        if (cap < need) {
            size_t new_cap = cap ? cap : 16;
            while (new_cap < need) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* allocation failure */
            terms = tmp;
            cap   = new_cap;
        }
        for (size_t i = sz; i < need; ++i) {
            while (!has_equal_5_7(next_value)) ++next_value;
            terms[i] = next_value;
            ++next_value;
        }
        sz = need;
    }

    return terms[(size_t)(n - 1)];
}
