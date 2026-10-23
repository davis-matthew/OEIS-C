/*
 * A121317  Minimum molecular topological index for alkane trees with n
 *          vertices.
 * https://oeis.org/A121317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n^2 - 8*n + 4 (for n ≤ 5) For n ≥ 6: a(n) = 4*n^2 - 8*n
 *            + 4 + 27*g^2 + 9*g + 6 + (6 + 18*g)*r where g = (n - 6) / 3
 *            (integer division) r = (n - 6) % 3 This reproduces the OEIS
 *            sequence A121317 exactly
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first few terms directly. */
    if (n <= 5) {
        return (seq_t)4 * n * n - (seq_t)8 * n + 4;
    }

    /* Offset from n = 6. */
    idx_t m = n - 6;
    idx_t g = m / 3;     /* group index   */
    idx_t r = m % 3;     /* position inside the group */

    /* Base quadratic term 4*n^2 - 8*n + 4 */
    seq_t base = (seq_t)4 * n * n - (seq_t)8 * n + 4;

    /* Extra contribution from the piecewise linear part. */
    seq_t gg = g;        /* promote to seq_t for arithmetic */
    seq_t rr = r;
    seq_t extra = (seq_t)27 * gg * gg + (seq_t)9 * gg + 6
                  + ((seq_t)6 + (seq_t)18 * gg) * rr;

    return base + extra;
}
