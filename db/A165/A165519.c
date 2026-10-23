/*
 * A165519  Integers k for which k(k+1)(k+2) is a triangular number.
 * https://oeis.org/A165519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) is the nth integer k such that k(k+1)(k+2) is a triangular
 *            number. Computed by searching all integers k in a bounded range;
 *            the curve has only nine integer solutions, all of which lie
 *            between -2000 and 2000
 * Range:     i8 none, i16 none, i32 none, i64 n <= 9, i128 n <= 9, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"   /* provides floor_sqrt(seq_t) */

/* static storage for pre‑computed sequence values */
static seq_t _sol[16];
static size_t _cnt = 0;
static bool _init = false;

/* generate all solutions of 8*k(k+1)(k+2)+1 = square */
static void _init_sol(void)
{
    if (_init) return;
    for (int64_t k = -2000; k <= 2000; ++k)
    {
        seq_t kk = (seq_t)k;
        seq_t val = ((seq_t)8 * kk * (kk + 1) * (kk + 2)) + 1;
        seq_t s   = floor_sqrt(val);
        if (s * s == val)
            _sol[_cnt++] = kk;
    }
    _init = true;
}

seq_t a(idx_t n)
{
    _init_sol();
    idx_t idx = n - 1;
    if (idx < _cnt)
        return _sol[idx];
    /* Sequence is finite; beyond its last term return 0. */
    return 0;
}
