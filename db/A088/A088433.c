/*
 * A088433  Number of ways to write n as n = u*v*w with 1<=u<=v<w.
 * https://oeis.org/A088433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (u,v,w) with 1 <= u <= v < w and u*v*w
 *            = n
 * Range:     u8 n <= 2048, u16 n <= 2048, u32 n <= 2048, u64 n <= 2048,
 *            u128 n <= 2048, big any n
 * Helpers:   int_cube_root, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_cube_root.h"
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    seq_t count = 0;
    idx_t umax = int_cube_root(n);          /* floor(cuberoot(n))         */
    for (idx_t u = 1; u <= umax; ++u) {
        if (n % u != 0) continue;           /* u must divide n            */
        idx_t n1 = n / u;                   /* remaining product v*w      */

        idx_t vmax = int_sqrt(n1);          /* floor(sqrt(n1))           */
        for (idx_t v = u; v <= vmax; ++v) {
            if (n1 % v != 0) continue;      /* v must divide n1          */
            idx_t w = n1 / v;                /* w = n1 / v                 */
            if (v < w) ++count;              /* enforce v < w              */
        }
    }
    return count;
}
