/*
 * A022413  Kim-sums: "Kimberling sums" K_n + K_2.
 * https://oeis.org/A022413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 0, otherwise a(n) = A000201(n) + n + 3, where
 *            A000201(n) = floor(n*φ) = (n + floor_sqrt(5*n*n))/2
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9950 (2026-11-01)
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

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    seq_t n_seq = (seq_t)n;
    seq_t n_sq   = n_seq * n_seq;           /* n² */
    seq_t f_nat  = n_sq * (seq_t)5;         /* 5·n² */
    seq_t m      = floor_sqrt(f_nat);       /* floor(√(5·n²)) = floor(n·√5) */
    seq_t low    = (n_seq + m) / (seq_t)2;  /* floor(n·φ) */

    return low + n_seq + (seq_t)3;
}
