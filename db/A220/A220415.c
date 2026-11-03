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
 * Validated: standard, pass, 82 terms through n = 9501 (2026-11-01)
 * Helpers:   isqrt_u128_to_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Antidiagonal enumeration of T(n,k) = floor(n/k)+floor(k/n) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_u128_to_u64.h"

/* Convert sequence index n to indices (i,j) of the triangle */
static inline void find_ij(idx_t n, idx_t *i, idx_t *j)
{
    /* Compute d = ceil( (sqrt(8*n+1)-1)/2 )  */
    unsigned __int128 s = (unsigned __int128)8 * n + 1;
    uint64_t sqrt_s = isqrt_u128_to_u64(s);
    uint64_t d0 = (sqrt_s - 1) / 2;
    unsigned __int128 sum_d0 = (unsigned __int128)d0 * (d0 + 1) / 2;
    uint64_t d = (sum_d0 >= n) ? d0 : d0 + 1;

    /* Number of terms before the d‑th antidiagonal */
    unsigned __int128 prev = (unsigned __int128)(d - 1) * d / 2;
    idx_t p = n - (idx_t)prev;       /* position within this diagonal (1‑based) */
    *i = d - p + 1;                  /* i decreases from d to 1 */
    *j = p;                          /* j increases from 1 to d */
}

seq_t a(idx_t n)
{
    idx_t i, j;
    find_ij(n, &i, &j);

    idx_t t1 = i / j;   /* floor(i/j)  */
    idx_t t2 = j / i;   /* floor(j/i)  */
    return (seq_t)(t1 + t2);
}
