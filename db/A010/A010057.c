/*
 * A010057  a(n) = 1 if n is a cube, else 0.
 * https://oeis.org/A010057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 if n is a perfect cube, otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    /* Compute integer cube root of n */
    idx_t r = int_cube_root(n);

    /* r^3 fits in idx_t because r^3 <= n < 2^64 */
    idx_t cube = r * r * r;

    /* Return 1 if n is a perfect cube, else 0 */
    return (seq_t)(n == cube);
}
