/*
 * A040919  Continued fraction for sqrt(950).
 * https://oeis.org/A040919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 89 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the continued‑fraction coefficients of sqrt(950) */
#include <stdbool.h>
#include <stddef.h>
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

/* temporary storage for the period and the first term of the expansion */
static seq_t a0_val;
static seq_t period[128];
static idx_t period_len = 0;
static bool initialized = false;

/* compute the period of the continued fraction of sqrt(950) once */
static void init_period(void) {
    if (initialized)
        return;
    const seq_t D = 950;
    a0_val = floor_sqrt(D);          /* a0 = floor(sqrt(950)) */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0_val;
    period_len = 0;
    while (1) {
        /* recurrence: m_{k+1} = d_k * a_k - m_k */
        m = d * a - m;
        /* d_{k+1} = (D - m_{k+1}^2) / d_k */
        d = (D - m * m) / d;
        /* a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1}) */
        a = (a0_val + m) / d;
        /* store a_{k+1} (which is a1, a2, …) */
        period[period_len++] = a;
        /* the period ends when a_k = 2*a0 */
        if (a == 2 * a0_val)
            break;
    }
    initialized = true;
}

/* exported function required by the framework */
seq_t a(idx_t n) {
    init_period();
    if (n == 0)
        return a0_val;                 /* a(0) = floor(sqrt(950)) = 30 */
    idx_t idx = (n - 1) % period_len;  /* subsequent terms repeat */
    return period[idx];
}
