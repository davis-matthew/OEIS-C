/*
 * A130239  Maximal index k of the square of a Fibonacci number such that
 *          Fib(k)^2 <= n (the 'lower' squared Fibonacci Inverse).
 * https://oeis.org/A130239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max k such that Fib(k)^2 <= n (A130239)
 * Range:     u8 n <= 63, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 121 terms through n = 4732 (2026-11-01)
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

/* Return the maximal index k with Fib(k)^2 <= n.
 * n is given as an idx_t (unsigned 64-bit); the value returned
 * is a seq_t (unsigned or arbitrary-precision integer).
 */
seq_t a(idx_t n)
{
    /* Convert n to seq_t and take floor of its square root. */
    seq_t sqrt_n = floor_sqrt((seq_t)n);

    /* Compute Fibonacci numbers until the next one would exceed sqrt_n. */
    seq_t f_prev = 0;   /* Fib(0) */
    seq_t f_curr = 1;   /* Fib(1) */
    idx_t idx   = 1;    /* current Fibonacci index   */

    while (f_curr <= sqrt_n) {
        seq_t f_next = f_prev + f_curr;  /* Fib(k+1) */
        f_prev = f_curr;
        f_curr = f_next;
        idx++;    /* move to next index */
    }

    /* idx is one past the desired k, so subtract 1. */
    return (seq_t)(idx - 1);
}
