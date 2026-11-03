/*
 * A186352  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f and g are the odd numbers and the
 *          triangular numbers. Complement of A186353.
 * https://oeis.org/A186352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(-1/2 + sqrt(4n-3/4))
 * Range:     u8 n <= 4, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Validated: partial, pass, 120 terms through n = 120 (2026-11-01)
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

seq_t a(idx_t n)
{
    /* Handle offset: a(1) is first term. For n = 0 return 0. */
    if (n == 0) return 0;

    /* Convert index to the general integer type seq_t. */
    seq_t n_seq = (seq_t)n;

    /* Evaluate 16*n - 3, which equals 4*(4*n) - 3 = 4*(4n) - 3.
       This is the number inside the square‑root in the formula. */
    seq_t arg = (seq_t)16 * n_seq - (seq_t)3;

    /* Integer square‑root of the argument. */
    seq_t s = floor_sqrt(arg);

    /* Compute floor( (sqrt(arg) - 1) / 2 ) as (s - 1) / 2. */
    seq_t shift = (s - (seq_t)1) / (seq_t)2;

    /* Final value: n + shift. */
    return n_seq + shift;
}
