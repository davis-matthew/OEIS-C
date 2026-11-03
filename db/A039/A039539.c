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
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑12 representation has the same nonzero
   number of 5's and 7's (OEIS A039539) */
#include <stdlib.h>
#include "../utils/mem.h"

static inline int has_equal_5_7(seq_t v)
{
    int c5 = 0, c7 = 0;
    const seq_t base = (seq_t)12;
    while (v > 0) {
        int d = (int)(v % base);
        if (d == 5) ++c5;
        else if (d == 7) ++c7;
        v = v / base;
    }
    return (c5 == c7) && (c5 > 0);
}

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
            seq_t *tmp = mem_resize(seq_t, terms, new_cap);
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
