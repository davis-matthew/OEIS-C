/*
 * A004746  Numbers whose binary expansion does not contain 010.
 * https://oeis.org/A004746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   search -- a(n) is the n-th number k >= 0 (in increasing order)
 *            whose binary expansion has no "010"
 * Range:     u8 n <= 150, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9987 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool contains_010(uint64_t x) {
    while (x >= 4) {
        if ((x & 7) == 2)
            return true;
        x >>= 1;
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t current = 0;
    for (;;) {
        if (!contains_010(current)) {
            count++;
            if (count == n)
                return (seq_t)current;
        }
        current++;
    }
}
