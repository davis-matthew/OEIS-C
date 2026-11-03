/*
 * A061433  Largest n-digit square.
 * https://oeis.org/A061433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (ceiling(10^(n/2)) - 1)^2, equivalent to the largest
 *            n‑digit perfect square
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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

/* Returns the largest square with exactly n decimal digits.
   n >= 1. */
seq_t a(idx_t n)
{
    /* 10^n as a seq_t. */
    seq_t pow10n = pow10(n);

    /* 10^n - 1.  The integer type conversion of 1 to seq_t
       works for all allowed seq_t definitions. */
    seq_t max_n_digit = pow10n - (seq_t)1;

    /* Floor of the square root of that number. */
    seq_t root = floor_sqrt(max_n_digit);

    /* The desired square. */
    return root * root;
}
