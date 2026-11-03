/*
 * A185599  a(n) = floor(n^(3/2) - n^(1/2)) - n; complement of A185600.
 * https://oeis.org/A185599
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^(3/2) - n^(1/2)) - n
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 80, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 94 terms through n = 4839 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation is done by observing that 
   (n-1)*sqrt(n) = sqrt((n-1)^2 * n). 
   Hence a(n) = floor(sqrt((n-1)^2 * n)) - n. */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n < 3) /* offset is 3; behaviour below offset is irrelevant */ 
        return (seq_t)0;

    seq_t N  = (seq_t)n;   /* convert index to seq_t */
    seq_t n1 = N - 1;      /* n-1 */

    /* compute (n-1)^2 * n in seq_t arithmetic   */
    seq_t temp = n1 * n1 * N;

    /* floor(sqrt(temp)) gives floor((n-1)*sqrt(n)) */
    seq_t s = floor_sqrt(temp);

    return s - N;
}
