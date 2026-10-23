/*
 * A175362  Number of integer pairs (x,y) satisfying |x|^3 + |y|^3 = n, -n <=
 *          x,y <= n.
 * https://oeis.org/A175362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer pairs (x,y) with |x|^3 + |y|^3 = n, -n
 *            <= x,y <= n
 * Range:     u8 n <= 5146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_cube_root.h"   /* provides idx_t int_cube_root(idx_t) */

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == 0) return (seq_t)1;

    seq_t count = 0;
    idx_t max_x = int_cube_root(n);        /* floor(cuberoot n) */

    for (idx_t x = 0; x <= max_x; ++x) {
        /* x^3 fits in seq_t because x^3 <= n */
        seq_t x3 = (seq_t)x * x * x;
        seq_t rem = (seq_t)n - x3;        /* rem = n - x^3 >= 0 */

        /* rem <= n, so fits in idx_t */
        idx_t y = int_cube_root((idx_t)rem);
        seq_t y3 = (seq_t)y * y * y;

        if (y3 == rem) {                 /* |y|^3 == rem  */
            if (x == 0 && y == 0) {
                count += 1;               /* (0,0) */
            } else if (x == 0 || y == 0) {
                count += 2;               /* one zero, two signs on non‑zero */
            } else {
                count += 4;               /* both non‑zero, 4 sign combinations */
            }
        }
    }

    return count;
}
