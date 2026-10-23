/*
 * A192827  Molecular topological indices of the gear graphs.
 * https://oeis.org/A192827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*n*(3*n-2)
 * Range:     u8 n <= 3, u16 n <= 44, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* compute 3*n - 2 safely using shifts: 3*n = (n<<1) + n */
    seq_t t = (nn << 1) + nn - 2;
    return 11 * nn * t;
}
