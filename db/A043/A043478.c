/*
 * A043478  Numbers having two 6's in base 9.
 * https://oeis.org/A043478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having exactly two 6's in base 9 representation. */

#include <stdbool.h>

static bool has_two_sixes(seq_t x) {
    idx_t count = 0;
    seq_t nine = (seq_t)9;
    while (x > 0) {
        seq_t digit = x % nine;
        if (digit == (seq_t)6) {
            count++;
            if (count > 2) return false;
        }
        x /= nine;
    }
    return count == 2;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset 1, undefined for n < 1 */
    seq_t current = 1;
    idx_t found = 0;
    while (found < n) {
        if (has_two_sixes(current))
            found++;
        current++;
    }
    return current - 1;
}
