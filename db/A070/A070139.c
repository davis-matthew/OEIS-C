/*
 * A070139  Number of isosceles integer triangles with perimeter n having
 *          integral area.
 * https://oeis.org/A070139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of isosceles integer triangles with perimeter n
 *            having integral area
 * Range:     u8 n <= 7, u16 n <= 25, u32 n <= 369, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* Small n cannot form a valid isosceles triangle */
    if (n <= 5) return 0;

    idx_t a_min = n / 4 + 1;          /* a > n/4  */
    idx_t a_max = (n - 1) / 2;        /* a < n/2  */

    for (idx_t a = a_min; a <= a_max; ++a) {
        idx_t b = n - 2 * a;          /* base length */
        /* Compute n * b^2 * (4a - n)  */
        seq_t tmp = (seq_t)n * (seq_t)b * (seq_t)b *
                     (((seq_t)4 * a) - (seq_t)n);

        seq_t s = floor_sqrt(tmp);
        if (s * s == tmp && (s & 3) == 0) {
            /* sqrt(tmp) is multiple of 4 → area is an integer */
            ++count;
        }
    }
    return count;
}
