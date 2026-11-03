/*
 * A034989  Smallest squares starting with a string of n 6's.
 * https://oeis.org/A034989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest square starting with a string of n 6's. */
/* The function searches for the minimal number of trailing zeros t such that
   the interval [P·10^t, (P+1)·10^t) contains a perfect square, where P is the
   integer consisting of n consecutive 6's. */
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

/* Test whether a square exists in the interval [P*10^t, (P+1)*10^t). */
static bool
check_t(idx_t t, const seq_t *P, const seq_t *P1)
{
    seq_t pow_t = pow10(t);
    seq_t low  = (*P) * pow_t;         /* P * 10^t */
    seq_t high = (*P1) * pow_t;        /* (P+1) * 10^t */
    seq_t k = floor_sqrt(low);         /* floor(sqrt(low)) */
    if (k * k < low)                  /* ceil(sqrt(low)) if needed */
        k = k + 1;
    return k * k < high;               /* square falls inside the interval? */
}

seq_t
a(idx_t n)
{
    /* Build the integer consisting of n 6's: P = 666...6 (n times). */
    seq_t P = 0;
    for (idx_t i = 0; i < n; ++i)
        P = P * 10 + 6;
    seq_t P1 = P + 1;

    /* Binary search for the minimal t (0 ≤ t ≤ n+1) that admits a square. */
    idx_t lo = 0, hi = n + 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (check_t(mid, &P, &P1))
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t t = lo;

    /* Compute the corresponding square (the minimal one for this t). */
    seq_t pow_t = pow10(t);
    seq_t low  = P * pow_t;
    seq_t k = floor_sqrt(low);
    if (k * k < low)
        k = k + 1;
    return k * k;
}
