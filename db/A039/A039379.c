/*
 * A039379  Numbers whose base-9 representation has the same nonzero number of
 *          3's and 5's.
 * https://oeis.org/A039379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9236 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-9 representation has the same nonzero number of 3's and 5's.
   This function scans successive integers, counts digits 3 and 5 in base 9,
   and returns the nth integer (1‑based) satisfying the condition. */
#include <stdbool.h>

/* test whether a number has the same positive number of 3's and 5's in base 9 */
static inline bool has_equal_nonzero_35(seq_t x)
{
    unsigned cnt3 = 0, cnt5 = 0;
    while (x > (seq_t)0) {
        unsigned digit = (unsigned)(x % (seq_t)9);
        if (digit == 3) cnt3++;
        else if (digit == 5) cnt5++;
        x = x / (seq_t)9;
    }
    return (cnt3 == cnt5) && (cnt3 > 0);
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cand   = 0;
    while (1) {
        cand = cand + (seq_t)1;
        if (has_equal_nonzero_35(cand)) {
            found++;
            if (found == n) return cand;
        }
    }
}
