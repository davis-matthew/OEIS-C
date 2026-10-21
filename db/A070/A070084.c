/*
 * A070084  Greatest common divisor of sides of integer triangles [A070080(n),
 *          A070081(n), A070082(n)], sorted by perimeter, sides
 *          lexicographically ordered.
 * https://oeis.org/A070084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the GCD of the sides of the n‑th integer triangle sorted by
   perimeter (ascending) and then lexicographically (ascending a, then b). */
#include <stdint.h>

static inline seq_t gcd_local(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a   = b;
        b   = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n<1, safe guard */

    idx_t count = 0; /* number of triangles seen so far */

    for (idx_t p = 3; ; ++p) {          /* perimeter starts at 3 */
        idx_t max_a = p / 3;             /* a ≤ p/3 to keep a ≤ b ≤ c possible */

        for (idx_t a_val = 1; a_val <= max_a; ++a_val) {
            idx_t max_b = (p - a_val) / 2;     /* b ≤ (p−a)/2 to satisfy b ≤ c */

            for (idx_t b_val = a_val; b_val <= max_b; ++b_val) {
                idx_t c_val = p - a_val - b_val;
                if (a_val + b_val <= c_val) continue;  /* triangle inequality */

                ++count;
                if (count == n) {
                    seq_t side1 = (seq_t)a_val;
                    seq_t side2 = (seq_t)b_val;
                    seq_t side3 = (seq_t)c_val;
                    seq_t g = gcd_local(side1, side2);
                    g = gcd_local(g, side3);
                    return g;
                }
            }
        }
    }

    return 0; /* unreachable */
}
