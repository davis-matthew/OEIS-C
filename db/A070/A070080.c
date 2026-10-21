/*
 * A070080  Smallest side of integer triangles [a(n) <= A070081(n) <=
 *          A070082(n)], sorted by perimeter, lexicographically ordered.
 * https://oeis.org/A070080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A070080(n) = smallest side of integer triangles sorted by
 *            perimeter, then lexicographically. This implementation
 *            enumerates all triangles (a ≤ b ≤ c, a+b>c), ordered by
 *            increasing perimeter and lexicographically, and returns the nth
 *            such triangle's smallest side
 * Range:     u8 n <= 1285, u16 n <= 1285, u32 n <= 1285, u64 n <= 1285,
 *            u128 n <= 1285, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t count = 0;                     // number of triangles found
    for (idx_t p = 3; ; ++p) {           // perimeter starts at 3 (1+1+1)
        seq_t p_s = (seq_t)p;            // for arithmetic in seq_t
        /* a ranges from 1 to floor(p/3) */
        seq_t a_max = p_s / 3;                          
        for (seq_t a = 1; a <= a_max; ++a) {
            /* b ranges from a to floor((p-a)/2) to ensure a ≤ b ≤ c */
            seq_t b_max = (p_s - a) / 2;
            for (seq_t b = a; b <= b_max; ++b) {
                seq_t c = p_s - a - b;                // remaining side
                if (a + b <= c) continue;            // violates triangle inequality
                ++count;
                if (count == n) return a;            // nth triangle found
            }
        }
    }
}
