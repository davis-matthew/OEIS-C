/*
 * A069659  Largest n-digit perfect power.
 * https://oeis.org/A069659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(sqrt(10^n – 1))^2 – largest n‑digit perfect power
 *            (always a square)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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

seq_t a(idx_t n)
{
    if (n < 1)           /* offset is 1 – behaviour for n < 1 is irrelevant */
        return (seq_t)0;

    /* compute 10^n as a seq_t value */
    seq_t tenpow = pow10(n);

    /* largest n‑digit number: 10^n – 1 */
    seq_t max_num = tenpow - (seq_t)1;

    /* floor of the square root of that number */
    seq_t root = floor_sqrt(max_num);

    /* return its square, the largest perfect power with n digits */
    return root * root;
}
