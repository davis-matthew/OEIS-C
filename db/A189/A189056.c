/*
 * A189056  Numbers having in decimal representation at least one common digit
 *          with their squares.
 * https://oeis.org/A189056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth number m ≥ 0 such that decimal digits of m and m²
 *            intersect
 * Range:     u8 n <= 10, u16 n <= 973, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool common_digit(seq_t x, seq_t y) {
    bool seen[10] = {0};
    seq_t t = x;
    /* Process digits of x, handling the case x == 0 */
    do {
        unsigned d = (unsigned)(t % 10);
        seen[d] = true;
        t /= 10;
    } while (t > 0);
    t = y;
    /* Process digits of y, returning true on first common digit */
    do {
        unsigned d = (unsigned)(t % 10);
        if (seen[d]) return true;
        t /= 10;
    } while (t > 0);
    return false;
}

seq_t a(idx_t n) {
    seq_t target = (seq_t)n;
    seq_t count  = 0;
    for (seq_t i = 0;; ++i) {
        seq_t sq = i * i;
        if (common_digit(i, sq)) {
            ++count;
            if (count == target)
                return i;
        }
    }
    return 0; /* unreachable */
}
