/*
 * A040257  Continued fraction for sqrt(274).
 * https://oeis.org/A040257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: terms of the continued fraction expansion of sqrt(274).
 *  a(0) = ⌊√274⌋ (=16).  For n≥1, a(n) is produced by the standard
 *  continued‑fraction algorithm for √274, whose period is 7:
 *  (1, 1, 4, 4, 1, 1, 32) repeating after a(0).
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t _cf_sqrt274_period[64];
static size_t _cf_sqrt274_period_len = 0;
static bool _cf_sqrt274_inited = false;

/* integer square root of a non‑negative 64‑bit integer (floor) */
static seq_t _int_sqrt_u64(idx_t v)
{
    seq_t lo = 0, hi = (seq_t)v + 1;
    while (lo + 1 < hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        if ((uint64_t)mid * (uint64_t)mid <= v)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* compute the period of the continued fraction for sqrt(274) */
static void _init_cf_sqrt274(void)
{
    if (_cf_sqrt274_inited)
        return;

    const uint64_t D = 274ull;
    const seq_t a0 = _int_sqrt_u64(D);  /* floor sqrt(274) = 16 */

    seq_t m = 0, d = 1, a = a0;
    size_t idx = 0;

    while (true) {
        m = d * a - m;
        /* D - m*m always fits in 64 bits for this D */
        d = (D - (unsigned __int128)m * (unsigned __int128)m) / d;
        a = (a0 + m) / d;   /* floor((a0 + m)/d) */
        _cf_sqrt274_period[idx++] = a;
        if (a == 2 * a0)   /* period terminator */
            break;
    }
    _cf_sqrt274_period_len = idx;
    _cf_sqrt274_inited = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return _int_sqrt_u64(274ull); /* a(0) = 16 */

    _init_cf_sqrt274();               /* ensure period is ready */

    /* pattern repeats every _cf_sqrt274_period_len terms
       starting from a(1) */
    size_t pos = (size_t)((n - 1) % _cf_sqrt274_period_len);
    return _cf_sqrt274_period[pos];
}
