/*
 * A061054  a(n) = floor(n + n^(3/4)).
 * https://oeis.org/A061054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n + n^(3/4)⌋
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 70 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the integer part of n^(3/4) by evaluating
   the fourth root of n^3, using only integer arithmetic.  The result is
   then added to n (ignoring the fractional part).  All intermediate
   computations are performed in 128‑bit integer to avoid overflow for
   the values expected in the test set. */
#include <stdint.h>

static seq_t floor_n_pow_3_4(idx_t n)
{
    if (n == 0)
        return 0; /* 0^(3/4) = 0 */

    /* Compute n^3 in 128‑bit to avoid overflow */
    __int128 nx = (__int128)n * n * n;  /* n^3 */

    /* Binary search for the greatest x with x^4 <= n^3.
       We can safely set upper bound to n, because n^4 > n^3 for n>0. */
    idx_t lo = 0;
    idx_t hi = n;

    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        __int128 mid4 = (__int128)mid * mid * mid * mid;
        if (mid4 <= nx)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo; /* floor(n^(3/4)) */
}

seq_t a(idx_t n)
{
    seq_t term = (seq_t)n + floor_n_pow_3_4(n);
    return term;
}
