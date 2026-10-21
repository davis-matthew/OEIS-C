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
 * Helpers:   count_triangles
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/count_triangles.h"

seq_t a(idx_t n) {
    seq_t cumulative = 0;
    for (idx_t p = 3; ; ++p) {
        idx_t cnt = count_triangles(p);
        if (cumulative + cnt >= n)
            return (seq_t)p;
        cumulative += cnt;
    }
}
