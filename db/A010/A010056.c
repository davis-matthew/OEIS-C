/*
 * A010056  Characteristic function of Fibonacci numbers: a(n) = 1 if n is a
 *          Fibonacci number, otherwise 0.
 * https://oeis.org/A010056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is 1 if n is a Fibonacci number, 0 otherwise
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10946, u64 n <= 10946,
 *            u128 n <= 10946, big any n
 * Validated: standard, pass, 122 terms through n = 8963 (2026-11-01)
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

static seq_t is_square(seq_t x)
{
    seq_t s = floor_sqrt(x);
    return (s * s == x) ? (seq_t)1 : (seq_t)0;
}

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;          /* n^2 */
    seq_t tmp = n2 * (seq_t)5;    /* 5*n^2 */
    seq_t val1 = tmp + (seq_t)4;  /* 5*n^2 + 4 */
    if (is_square(val1)) return (seq_t)1;
    if (n > 0)
    {
        seq_t val2 = tmp - (seq_t)4; /* 5*n^2 - 4 */
        if (is_square(val2)) return (seq_t)1;
    }
    return (seq_t)0;
}
