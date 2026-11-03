/*
 * A000924  Class number of Q(sqrt(-n)), n squarefree.
 * https://oeis.org/A000924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = h(-m), where m is the n-th squarefree number (A005117)
 *            and h is the class number of Q(sqrt(-m)). The code finds m by
 *            search, then counts the reduced forms (a,b,c) with 4ac - b^2 =
 *            |D|, where D = -m if m == 3 (mod 4) and D = -4m otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 114 terms through n = 9628 (2026-11-01)
 * Helpers:   is_squarefree_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_squarefree_u64.h"

static seq_t a924_class_number(uint64_t d) {
    int64_t D = (d % 4 == 3) ? (int64_t)d : 4 * (int64_t)d;
    seq_t count = 0;
    /* reduced: |b| <= a <= c, so 3a^2 <= D */
    for (int64_t a = 1; 3 * a * a <= D; a++) {
        for (int64_t b = -a + 1; b <= a; b++) {
            int64_t num = D + b * b;
            int64_t den = 4 * a;
            if (num % den != 0) continue;
            int64_t c = num / den;
            if (c < a) continue;
            if (c == a && b < 0) continue;
            count++;
        }
    }
    return count;
}

static uint64_t a924_nth_sqfree(idx_t n) {
    uint64_t count = 0, m = 0;
    while (count < (uint64_t)n) {
        m++;
        if (is_squarefree_u64(m)) count++;
    }
    return m;
}

seq_t a(idx_t n) {
    return a924_class_number(a924_nth_sqfree(n));
}
