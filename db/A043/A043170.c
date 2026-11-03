/*
 * A043170  Numbers k such that 3 and 4 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: find the nth positive integer k such that the octal
   representation of k contains adjacent digits 3 and 4 (in either
   order) and the representation of k‑1 does not contain such a pair. */
#include <stdbool.h>

static inline bool has_adjacent34(seq_t x)
{
    /* Detect if octal representation of x has 3 and 4 adjacent (34 or 43). */
    seq_t prev = x & (seq_t)7;   /* least significant octal digit */
    x >>= 3;
    while (x) {
        seq_t cur = x & (seq_t)7;
        if ((prev == (seq_t)3 && cur == (seq_t)4) ||
            (prev == (seq_t)4 && cur == (seq_t)3))
            return true;
        prev = cur;
        x >>= 3;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t k = 0;
    while (1) {
        ++k;                                  /* consider next integer */
        if (has_adjacent34(k)) {
            if (!has_adjacent34(k - (seq_t)1)) {
                ++found;
                if (found == n)
                    return k;
            }
        }
    }
}
