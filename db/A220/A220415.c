/*
 * A220415  Table T(n,k)= floor(n/k)+ floor(k/n), n,k >0 read by
 *          antidiagonals.
 * https://oeis.org/A220415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   find_ij, u128_floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Antidiagonal enumeration of T(n,k) = floor(n/k)+floor(k/n) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/find_ij.h"
#include "../utils/u128_floor_sqrt.h"

seq_t a(idx_t n)
{
    idx_t i, j;
    find_ij(n, &i, &j);

    idx_t t1 = i / j;   /* floor(i/j)  */
    idx_t t2 = j / i;   /* floor(j/i)  */
    return (seq_t)(t1 + t2);
}
