/*
 * A070081  Middle side of integer triangles [A070080(n) <= a(n) <=
 *          A070082(n)], sorted by perimeter, sides lexicographically ordered.
 * https://oeis.org/A070081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A070083(n) - A070080(n) - A070082(n)
 * Range:     u8 n <= 1285, u16 n <= 1285, u32 n <= 1285, u64 n <= 1285,
 *            u128 n <= 1285, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the middle side of the n‑th integer triangle
   when all integer triangles (a ≤ b ≤ c, a + b > c, a + b + c = p)
   are ordered by increasing perimeter p and, for equal p,
   lexicographically by (a, b, c). */
#include <assert.h>

seq_t a(idx_t n)
{
    assert(n >= 1);
    idx_t count = 0;          /* how many triangles encountered so far */
    for (idx_t p = 3; ; ++p) {    /* perimeter */
        idx_t a_max = p / 3;          /* since a ≤ b ≤ c and a + b + c = p */
        for (idx_t a = 1; a <= a_max; ++a) {
            /* Compute the allowed range for b:
               a ≤ b ≤ c,
               a + b > c  ⇔  a + b > p - a - b  ⇔  a + b > p/2
               c  = p - a - b
               b ≤ c ⇔  b ≤ (p - a)/2
            */
            idx_t min_b = a;
            idx_t p2 = p / 2;           /* floor of p/2 */
            if (p2 > a) {                /* ensures p2 - a + 1 is non‑negative */
                idx_t cand = p2 - a + 1;
                if (cand > min_b) min_b = cand;
            }
            idx_t max_b = (p - a) / 2;
            if (min_b > max_b) continue; /* no valid b for this a */
            for (idx_t b = min_b; b <= max_b; ++b) {
                ++count;
                if (count == n) {
                    return (seq_t)b;    /* middle side of the n‑th triangle */
                }
            }
        }
    }
}
