/*
 * A004750  Numbers whose binary expansion contains the substring '011'.
 * https://oeis.org/A004750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 122, u16 n <= 32769, u32 n <= 32769, u64 n <= 32769,
 *            u128 n <= 32769, big any n
 * Validated: standard, pass, 74 terms through n = 8133 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool contains_011(seq_t num) {
    if (num < 4) return false;
    while (num >= 4) {
        if ((num & 7) == 3) {
            return true;
        }
        num >>= 1;
    }
    return false;
}

/* Search for the n-th positive integer whose binary expansion contains '011' */

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t num = 0;
    while (count < n) {
        num++;
        if (contains_011(num)) {
            count++;
        }
    }
    return num;
}
