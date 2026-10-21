/*
 * A092846  a(n) = 100...001^n, where there are just enough zeros for the
 *          result to display the terms in the n-th row of Pascal's triangle.
 * https://oeis.org/A092846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (10^k + 1)^n, k = digits( binom(n, floor(n/2)) )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   pow10, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/seq_pow.h"

static bool _init_logs = false;
#define MAX_LOG_N 20000
static long double _log_fact[MAX_LOG_N + 1];

static void _ensure_log_init(void)
{
    if (_init_logs) return;
    _log_fact[0] = 0.0L;
    for (idx_t i = 1; i <= MAX_LOG_N; ++i)
        _log_fact[i] = _log_fact[i - 1] + log10l((long double)i);
    _init_logs = true;
}

/* number of decimal digits in the central binomial coefficient C(n, floor(n/2)) */
static idx_t _central_binom_digits(idx_t n)
{
    _ensure_log_init();
    idx_t half = n >> 1;               /* floor(n/2) */
    long double val = _log_fact[n] - _log_fact[half] - _log_fact[n - half];
    /* small epsilon prevents mis‑count at integer boundaries */
    return (idx_t)(floorl(val + 1e-12L)) + 1;
}

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1;
    idx_t k = _central_binom_digits(n);
    seq_t base = pow10(k) + (seq_t)1;
    return seq_pow(base, n);
}
