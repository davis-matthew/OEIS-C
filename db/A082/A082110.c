/*
 * A082110  Array A(n,k) = (k*n)^2 + 5*(k*n) + 1, read by antidiagonals.
 * https://oeis.org/A082110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (i*j)^2 + 5*i*j + 1 where i = n − d(d+1)/2, * j = d − i,
 *            and d is the smallest integer with (d+1)(d+2)/2 > n. * This
 *            reproduces the OEIS sequence A082110 read by antidiagonals of *
 *            the square array A(n,k) = (k*n)^2 + 5*k*n + 1
 * Range:     u8 n <= 38, u16 n <= 542, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   tri_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/tri_u128.h"

seq_t a(idx_t n)
{
    /* Special‑case first term. */
    if (n == 0)
        return (seq_t)1;

    /* Find the smallest d such that T(d) > n.  tri_u128(d) > n. */
    uint64_t low = 0;
    uint64_t high = n;               /* tr(high) > n for all n >= 0 */
    while (low < high) {
        uint64_t mid = low + ((high - low) >> 1);
        if (tri_u128(mid) > (unsigned __int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    uint64_t d = low;                 /* first d with T(d) > n */

    /* Compute offset within the d‑th anti‑diagonal. */
    uint64_t T_prev = (uint64_t)tri_u128(d - 1); /* T(d-1) = d(d+1)/2 */
    uint64_t offset = n - T_prev;               /* 0 <= offset <= d */
    uint64_t i = offset;
    uint64_t j = d - offset;

    /* Compute the value using 128‑bit arithmetic for safety. */
    unsigned __int128 prod = (unsigned __int128)i * j;     /* i*j */
    unsigned __int128 val  = prod * prod + 5 * prod + 1;  /* (i*j)^2 + 5*i*j + 1 */

    return (seq_t)val;
}
