/*
 * A043962  Numbers k such that 0 and 3 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043962: numbers k such that the base‑9 representation of k contains the
   two adjacent digits 0 and 3 in any order, but the base‑9 representation
   of k+1 does not. This implementation scans integers from 1 upwards to find
   the nth such number. */

#include <stdbool.h>

static inline bool has_adj_0_3(seq_t num) {
    seq_t prev = 10;              /* sentinel > all base‑9 digits */
    while (num > 0) {
        seq_t digit = num % 9;
        num /= 9;
        if ((digit == 0 && prev == 3) || (digit == 3 && prev == 0))
            return true;
        prev = digit;
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t found = 0;
    seq_t k = 1;
    while (1) {
        if (has_adj_0_3(k) && !has_adj_0_3(k + 1)) {
            ++found;
            if (found == n)
                return k;
        }
        ++k;
    }
}
