/*
 * A040423  Continued fraction for sqrt(445).
 * https://oeis.org/A040423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 71 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued‑fraction coefficients of √445
   a(0) = ⌊√445⌋ = 21
   for n ≥ 1 the values repeat the period [10, 1, 1, 10, 42]        */
/* This function is independent of any external state and can be
   called in any order.  It precomputes the first term and the period
   using the standard continued‑fraction algorithm for quadratic
   irrationals. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

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

static bool _a040423_inited = false;
static seq_t _a040423_a0;
static seq_t *_a040423_period = NULL;
static size_t _a040423_period_len = 0;

static void _a040423_compute(void)
{
    const unsigned long long N = 445ULL;
    const unsigned long long SQRT_N = (unsigned long long)floor_sqrt((seq_t)N);
    _a040423_a0 = (seq_t)SQRT_N;

    unsigned long long m = 0ULL;
    unsigned long long d = 1ULL;
    unsigned long long a = SQRT_N;

    /* Allocate a small buffer for the period terms */
    size_t cap = 8;
    _a040423_period = mem_new(seq_t, cap);
    size_t len = 0;

    while (1) {
        unsigned long long m_next = d * a - m;
        unsigned long long d_next = (N - m_next * m_next) / d;
        unsigned long long a_next = (SQRT_N + m_next) / d_next;

        if (len >= cap) {
            cap *= 2;
            _a040423_period = mem_resize(seq_t, _a040423_period, cap);
        }
        _a040423_period[len++] = (seq_t)a_next;

        /* In the continued fraction for √N the period ends when a_next == 2*a0 */
        if (a_next == 2ULL * SQRT_N) {
            break;
        }
        m = m_next;
        d = d_next;
        a = a_next;
    }

    _a040423_period_len = len;
    _a040423_inited = true;
}

seq_t a(idx_t n)
{
    if (!_a040423_inited) {
        _a040423_compute();
    }
    if (n == 0ULL) {
        return _a040423_a0;
    }
    /* Period indices are 0‑based; shift n by 1 and wrap */
    size_t idx = ((size_t)(n - 1ULL)) % _a040423_period_len;
    return _a040423_period[idx];
}
