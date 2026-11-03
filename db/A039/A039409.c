/*
 * A039409  Numbers whose base-10 representation has the same nonzero number
 *          of 1's and 9's.
 * https://oeis.org/A039409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039409: numbers whose base‑10 representation has the same non‑zero
   number of 1's and 9's.
   Computed by brute‑force search. No simple closed form is known. */

#include <stdbool.h>

/* Return true if x contains an equal, non‑zero number of digits 1 and 9. */
static inline bool has_equal_ones_and_nines(seq_t x)
{
    idx_t c1 = 0, c9 = 0;
    while (x > 0) {
        int d = (int)(x % 10);   /* digit: 0..9, works for all seq_t types */
        if (d == 1) ++c1;
        else if (d == 9) ++c9;
        x /= 10;
    }
    return c1 == c9 && c1 > 0;
}

/* Return the nth value (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    for (seq_t v = 1;; ++v) {          /* start from the smallest candidate */
        if (has_equal_ones_and_nines(v)) {
            ++found;
            if (found == n)
                return v;
        }
    }
}
