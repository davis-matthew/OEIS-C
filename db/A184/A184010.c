/*
 * A184010  n + floor(sqrt(-1+4n/3)); complement of A001859 (except for
 *          initial zero).
 * https://oeis.org/A184010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(-1 + 4n/3))
 * Range:     u8 n <= 48, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* Compute the n-th term of OEIS sequence A184010.
 * a(n) = n + ⌊√(-1 + 4n/3)⌋, with offset 1. */
seq_t a(idx_t n) {
    /* cast the index to the arbitrary‑precision type */
    seq_t nn = (seq_t)n;

    /* compute (4*n - 3) / 3 taking the floor of the integer division */
    seq_t val = (4 * nn) - (seq_t)3;
    val /= (seq_t)3;    /* exact floor division for unsigned types */

    /* floor square root of the result */
    seq_t k = floor_sqrt(val);

    /* final value a(n) = n + k */
    return nn + k;
}
