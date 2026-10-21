/*
 * A221121  Number of n X 3 arrays of occupancy after each element stays put
 *          or moves to some horizontal or antidiagonal neighbor.
 * https://oeis.org/A221121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 26*a(n-1) - 41*a(n-2) (for n ≥ 3) Initial values: a(1) =
 *            8, a(2) = 200
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)200;

    seq_t a1 = (seq_t)8;
    seq_t a2 = (seq_t)200;
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t tmp = (seq_t)26 * a2 - (seq_t)41 * a1;
        a1 = a2;
        a2 = tmp;
    }
    return a2;
}
