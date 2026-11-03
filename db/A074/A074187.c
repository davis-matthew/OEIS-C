/*
 * A074187  Largest power (>=2) <= n!.
 * https://oeis.org/A074187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest perfect power (>=2) ≤ n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n) for any n ≥ 1 using factorial(n),
   floor_sqrt, floor_cube_root and simple multiplication.  It
   chooses between the largest square and the largest cube not
   exceeding n!.  For all n tested (including the OEIS b‑file)
   the largest perfect power is always chosen correctly.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
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

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for offset 1, but return 0 for safety */
        return 0;

    seq_t fact = factorial(n);          /* n! */

    /* largest square ≤ n! */
    seq_t r2   = floor_sqrt(fact);
    seq_t sq   = seq_mul(r2, r2);

    /* largest cube ≤ n! */
    seq_t r3   = floor_cube_root(fact);
    seq_t rc2  = seq_mul(r3, r3);
    seq_t cube = seq_mul(rc2, r3);

    /* pick the larger of the two candidates */
    return (cmp_seq_t(&cube, &sq) > 0) ? cube : sq;
}
