/*
 * A006092  Numbers beginning with letter 't' when spelled out in English.
 * https://oeis.org/A006092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn
 * Formula:   search: a(n) is the n-th k >= 1 whose leading 3-digit group (k /
 *            1000^j, below 1000) is spelled with a leading 't': 2, 3, 10, 12,
 *            13, 20-39, 200-399
 * Range:     u8 n <= 46, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool group_starts_with_t(seq_t g) {
    if (g >= (seq_t)100) {
        seq_t h = g / (seq_t)100;
        return h == (seq_t)2 || h == (seq_t)3;
    }
    if (g >= (seq_t)20) {
        seq_t t = g / (seq_t)10;
        return t == (seq_t)2 || t == (seq_t)3;
    }
    if (g >= (seq_t)10)
        return g == (seq_t)10 || g == (seq_t)12 || g == (seq_t)13;
    return g == (seq_t)2 || g == (seq_t)3;
}

static bool starts_with_t(seq_t k) {
    if (k == 0) return false;
    while (k >= (seq_t)1000) k /= (seq_t)1000;
    return group_starts_with_t(k);
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t i = 0;
    while (count < n) {
        i++;
        if (starts_with_t(i)) count++;
    }
    return i;
}
