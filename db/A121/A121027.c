/*
 * A121027  Multiples of 7 containing a 7 in their decimal representation.
 * https://oeis.org/A121027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth multiple of 7 whose decimal representation contains
 *            the digit 7
 * Range:     u8 n <= 6, u16 n <= 3157, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool contains_digit7(seq_t x) {
    while (x > 0) {
        if (x % 10 == 7)
            return true;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t m = 7;
    while (1) {
        if (contains_digit7(m)) {
            count++;
            if (count == n)
                return m;
        }
        m += 7;
    }
}
