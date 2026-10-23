/*
 * A032891  Numbers whose base-3 representation Sum_{i=0..m} d(i)*3^i has d(0)
 *          >= d(1) <= d(2) >= ...
 * https://oeis.org/A032891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑3 digits satisfy d0
 *            >= d1 <= d2 >= d3 <= d4
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t v = 1;
    for (;; ++v) {
        seq_t tmp = v;
        seq_t prev = tmp % 3;  /* d(0) */
        tmp /= 3;
        bool ok = true;
        int i = 0;             /* i = index of current digit (starting with 1) */
        while (tmp) {
            seq_t cur = tmp % 3;
            tmp /= 3;
            if ((i & 1) == 0) {          /* even step: require d(i-1) >= d(i) */
                if (prev < cur) { ok = false; break; }
            } else {                     /* odd step: require d(i-1) <= d(i) */
                if (prev > cur) { ok = false; break; }
            }
            prev = cur;
            ++i;
        }
        if (ok) {
            ++count;
            if (count == n) return v;
        }
    }
}
