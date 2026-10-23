/*
 * A175365  Number of integer triples (x,y,z) satisfying |x|^3 + |y|^3 + |z|^3
 *          = n, -n <= x,y,z <= n.
 * https://oeis.org/A175365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer triples (x,y,z) with |x|^3 + |y|^3 +
 *            |z|^3 = n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    /* We count ordered triples (a,b,c) of non‑negative integers such that
       a^3 + b^3 + c^3 = n.  For each triple the number of signed triples
       equals 2^{(# of non‑zero components)}.  Because we enumerate a and b
       independently, each ordered triple is counted exactly once.       */
    idx_t root = int_cube_root(n);          /* floor cubic root of n */
    seq_t count = 0;

    for (idx_t a = 0; a <= root; ++a) {
        idx_t ca = a * a * a;                /* a^3 */
        if (ca > n) break;                   /* safety, though a <= root */

        for (idx_t b = 0; b <= root; ++b) {
            idx_t cb = b * b * b;            /* b^3 */
            idx_t sum_ab = ca + cb;
            if (sum_ab > n) break;           /* further b will only increase */

            idx_t rem = n - sum_ab;          /* remaining value for c^3 */
            idx_t c = int_cube_root(rem);    /* floor cube root of rem */
            if (c * c * c == rem) {          /* exact cube match? */
                int k = 0;
                if (a != 0) ++k;
                if (b != 0) ++k;
                if (c != 0) ++k;
                seq_t sign = ((seq_t)1 << k); /* 2^k */
                count += sign;
            }
        }
    }
    return count;
}
