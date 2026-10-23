/*
 * A011533  Numbers that contain a 3.
 * https://oeis.org/A011533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer that contains the decimal digit
 *            '3' (OEIS A011533)
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool contains3(seq_t num) {
    if (num == 0) return false;
    for (seq_t tmp = num; tmp > 0; tmp /= 10) {
        if (tmp % 10 == 3) return true;
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t val = 0;
    for (;;) {
        if (contains3(val)) {
            ++count;
            if (count == n)
                return val;
        }
        val++;  /* assume seq_t is large enough for the required n */
    }
}
