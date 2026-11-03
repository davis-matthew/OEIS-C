/*
 * A186223  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f and g are the triangular numbers and
 *          pentagonal numbers. Complement of A186224.
 * https://oeis.org/A186223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊(1 + √(1 + 24·Tₙ))/6⌋ – I(Tₙ), where Tₙ = n·(n+1)/2
 *            and I(Tₙ)=1 iff Tₙ is also a pentagonal number
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 100, u64 n <= 100,
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

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t T = n_seq * (n_seq + 1) / 2;          /* Triangular number Tₙ */
    seq_t D = 1 + 24 * T;                       /* 1 + 24·Tₙ */
    seq_t sqrtD = floor_sqrt(D);                /* ⌊√D⌋ */
    seq_t pentCount = (sqrtD + 1) / 6;          /* ⌊(1+√D)/6⌋ */
    seq_t subtract = ((sqrtD * sqrtD == D) && ((sqrtD + 1) % 6 == 0)) ? 1 : 0;
    return n_seq + pentCount - subtract;
}
