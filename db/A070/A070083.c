/*
 * A070083  Perimeters of integer triangles, sorted by perimeter, sides
 *          lexicographically ordered.
 * https://oeis.org/A070083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = perimeter of the n‑th integer triangle sorted by
 *            perimeter (OEIS A070083)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static inline idx_t count_triangles(idx_t p) {
    idx_t max_a = p / 3;
    idx_t count = 0;
    for (idx_t a = 1; a <= max_a; ++a) {
        idx_t half = p >> 1;                    /* floor(p/2) */
        long long diff = (long long)half - (long long)a;   /* floor(p/2) - a */
        long long b_min_ll = (long long)a;
        if (diff + 1 > b_min_ll)
            b_min_ll = diff + 1;
        long long b_max_ll = ((long long)(p - a)) / 2;    /* floor((p - a)/2) */
        if (b_min_ll <= b_max_ll)
            count += (idx_t)(b_max_ll - b_min_ll + 1);
    }
    return count;
}

seq_t a(idx_t n) {
    seq_t cumulative = 0;
    for (idx_t p = 3; ; ++p) {
        idx_t cnt = count_triangles(p);
        if (cumulative + cnt >= n)
            return (seq_t)p;
        cumulative += cnt;
    }
}
