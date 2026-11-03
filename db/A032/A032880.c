/*
 * A032880  Numbers whose base-9 representation Sum_{i=0..m} d(i)*9^i has d(0)
 *          > d(1) < d(2) > ...
 * https://oeis.org/A032880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑9 representation has alternating inequalities
 *            d0 > d1 < d2 > d3 < … ; this function enumerates the sequence
 *            and returns the nth term (1‑based)
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool valid_pattern(seq_t v) {
    seq_t rem = v;
    int pos = 0;
    int prev = -1;          /* previous digit; unused at pos==0 */
    while (rem > 0) {
        int curr = (int)(rem % 9);   /* current digit (0…8) */
        rem /= 9;
        if (pos > 0) {
            if (pos & 1) {      /* odd position: previous > current */
                if (prev <= curr) return false;
            } else {            /* even position: previous < current */
                if (prev >= curr) return false;
            }
        }
        prev = curr;
        pos++;
    }
    return true;
}

seq_t a(idx_t n) {
    seq_t candidate = 1;
    idx_t count = 0;
    while (true) {
        if (valid_pattern(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        ++candidate;
    }
}
