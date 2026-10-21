/*
 * A059848  As a square table by antidiagonals, the n-digit number which in
 *          base k starts 1010101...
 * https://oeis.org/A059848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   a(n) is the n‑th entry of the infinite table T(i,j) (i,j ≥ 0)
 *            read by anti‑diagonals. T(0,k)=0, T(1,k)=1,
 *            T(i,k)=T(i-2,k)+k^(i-1) (i≥2) Documented by MIT
 * Range:     u8 n <= 49, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   T_of_i_j, floor_sqrt_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/T_of_i_j.h"
#include "../utils/floor_sqrt_uint64.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find anti‑diagonal d and offset i:
       n = d*(d+1)/2 + i,   0 ≤ i ≤ d.
    */
    uint64_t dd = floor_sqrt_uint64(8ULL * (unsigned long long)n + 1ULL);
    idx_t d = (idx_t)((dd - 1ULL) / 2ULL);

    /* adjust for possible rounding error of floating sqrt */
    while ((unsigned __int128)d * (d + 1) / 2 > n) --d;
    while ((unsigned __int128)(d + 1) * (d + 2) / 2 <= n) ++d;

    uint64_t base = (unsigned __int128)d * (d + 1) / 2;
    idx_t i = n - base;          /* row index within the diagonal */
    idx_t j = d - i;             /* column index */

    return T_of_i_j(i, j);
}
