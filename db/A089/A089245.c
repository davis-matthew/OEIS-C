/*
 * A089245  Integer part of square root of the reciprocal of a number
 *          multiplied by 10 to the power of the integer part of the square
 *          root of the number.
 * https://oeis.org/A089245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( sqrt(1/n)*10^(floor(sqrt(n))) )
 * Range:     u8 none, u16 n <= 8, u32 n <= 24, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A089245 for index n (n ≥ 1). */
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    seq_t n_seq = (seq_t)n;

    /* k = floor(sqrt(n)) */
    idx_t k = (idx_t)floor_sqrt(n_seq);

    /* 10^(2*k) */
    seq_t pow_val = pow10(2 * k);

    /* integer division: floor(10^(2*k) / n) */
    seq_t div_val = pow_val / n_seq;

    /* result = floor(sqrt(div_val)) */
    return floor_sqrt(div_val);
}
