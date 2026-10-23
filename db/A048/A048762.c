/*
 * A048762  Largest cube <= n.
 * https://oeis.org/A048762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (⌊n^(1/3)⌋)^3
 * Range:     u8 n <= 342, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    idx_t r = int_cube_root(n);   /* floor cube root of n   */
    seq_t rr = (seq_t)r;           /* promote to seq_t       */
    return rr * rr * rr;           /* r^3 as the largest cube ≤ n */
}
