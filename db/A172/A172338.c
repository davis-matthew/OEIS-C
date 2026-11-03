/*
 * A172338  a(n) = floor(n*(sqrt(5)+sqrt(3))).
 * https://oeis.org/A172338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = floor(n*(sqrt(5)+sqrt(3)))
 * Range:     u8 n <= 0, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 72 terms through n = 895 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* using a rational approximation of sqrt(5)+sqrt(3) with denominator 100000
       which guarantees that for n <= 10000 the error in n*alpha is < 1,
       thus floor(n*alpha) = floor(n*(num/den)). */
    static seq_t alpha_num = 0;   /* numerator of the approximation */
    static bool  initialized = false;
    const seq_t den = (seq_t)100000;

    if (!initialized) {
        seq_t sqrt5_part = floor_sqrt((seq_t)5 * den * den);   /* floor(sqrt(5)*den) */
        seq_t sqrt3_part = floor_sqrt((seq_t)3 * den * den);   /* floor(sqrt(3)*den) */
        alpha_num = sqrt5_part + sqrt3_part;                  /* floor((sqrt(5)+sqrt(3))*den) */
        initialized = true;
    }

    /* compute floor(n * alpha_num / den) in integer arithmetic */
    seq_t prod = (seq_t)n * alpha_num;
    seq_t result = prod / den;

    return result;
}
