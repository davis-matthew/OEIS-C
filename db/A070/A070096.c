/*
 * A070096  Number of integer triangles with perimeter n and relatively prime
 *          side lengths which are both acute and scalene.
 * https://oeis.org/A070096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of integer scalene triangles (a<b<c)
 *              with perimeter n that are acute and whose side lengths
 *              satisfy gcd(a,b,c)=1.
 *              Computation: for each admissible a,b compute c, test
 *              triangle inequality, acute condition a^2+b^2>c^2,
 *              and gcd(a,b,c)=1.
 */

#include <stdbool.h>
#include "../utils/gcd_seq.h"

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* triangles cannot exist for perimeter < 15 (minimum 4+5+6) */
    if (n < 15)
        return 0;

    /* ensure a<b<c and a+b+c=n */
    idx_t max_a = (n - 3) / 3;
    for (idx_t a_side = 1; a_side <= max_a; ++a_side) {
        /* b must satisfy a<b and c>a+b>... */
        idx_t b_min = a_side + 1;
        idx_t b_max = (n - a_side - 1) / 2;   /* guarantees c > b */
        for (idx_t b_side = b_min; b_side <= b_max; ++b_side) {
            seq_t c_side = (seq_t)n - (seq_t)a_side - (seq_t)b_side;

            /* triangle inequality */
            if ((seq_t)a_side + (seq_t)b_side <= c_side)
                continue;

            /* acute: a^2 + b^2 > c^2 */
            seq_t a2 = (seq_t)a_side * (seq_t)a_side;
            seq_t b2 = (seq_t)b_side * (seq_t)b_side;
            seq_t c2 = c_side * c_side;
            if (a2 + b2 <= c2)
                continue;

            /* gcd(a,b,c) == 1 */
            seq_t g_ab = gcd_seq((seq_t)a_side, (seq_t)b_side);
            if (gcd_seq(g_ab, c_side) != 1)
                continue;

            ++count;
        }
    }
    return count;
}
