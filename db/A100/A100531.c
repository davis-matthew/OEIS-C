/*
 * A100531  a(n) = a(n-1) + (2*n - 1) mod 8 + 1 with a(0)=1.
 * https://oeis.org/A100531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + ((2*n - 1) mod 8) + 1, a(0) = 1 * This
 *            implementation iterates from 1 to n, accumulating the
 *            increments. * The increment is computed using only bitwise
 *            operations to avoid * intermediate overflow on large indices
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Compute ((2*i - 1) mod 8) + 1 safely using unsigned arithmetic. */
        seq_t inc = ((((i & 3U) * 2U) + 7U) & 7U) + 1;  /* inc ∈ [1,8] */
        res += inc;
    }
    return res;
}
