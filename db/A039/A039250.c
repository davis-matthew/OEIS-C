/*
 * A039250  Numbers whose base-12 representation has the same number of 4's
 *          and 8's.
 * https://oeis.org/A039250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth nonnegative integer whose base‑12 representation
 *            contains an equal number of digits 4 and 8. Terms are indexed
 *            from 1 (a(1) = 0)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether a value has the same number of 4s and 8s in base 12. */
static inline bool equal_base12(seq_t v)
{
    unsigned cnt4 = 0, cnt8 = 0;
    if (v == 0)
        return true;              /* 0 has zero 4s and zero 8s */
    for (seq_t x = v; x > 0; x /= 12) {
        unsigned d = (unsigned)(x % 12);
        if (d == 4) ++cnt4;
        else if (d == 8) ++cnt8;
    }
    return cnt4 == cnt8;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    for (idx_t cand = 0;; ++cand) {
        if (equal_base12((seq_t)cand)) {
            ++found;
            if (found == n)
                return (seq_t)cand;
        }
    }
}
