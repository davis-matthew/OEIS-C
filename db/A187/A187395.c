/*
 * A187395  a(n) = floor(r*n), where r = 4 + sqrt(10); complement of A187396.
 * https://oeis.org/A187395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((4 + sqrt(10)) * n)
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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

/* Computes a(n) = floor(r*n), where r = 4 + sqrt(10) */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* n in target integer type */
    seq_t prod = sn * sn * (seq_t)10;   /* 10 * n^2  */
    seq_t f = floor_sqrt(prod);          /* floor(sqrt(10*n^2)) = floor(n*sqrt(10)) */
    return sn * (seq_t)4 + f;            /* 4*n + floor(n*sqrt(10)) */
}
