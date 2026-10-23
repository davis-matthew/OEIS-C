/*
 * A018483  Divisors of 490.
 * https://oeis.org/A018483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A018483 – Divisors of 490 in increasing order.  The function returns
   the nth divisor of 490 (1‑indexed).  For n > 12 it returns 0. */

/* No external libraries except the standard headers below. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Factorisation of 490 = 2¹ · 5¹ · 7² */
static const uint64_t _p_factors[3] = {2, 5, 7};
static const int        _p_exp[3]    = {1, 1, 2};
static seq_t           _divisors[12];
static bool           _init_done = false;

/* Recursive generation of divisors */
static void _gen_divisors(int idx, seq_t current, size_t *pos)
{
    if (idx == 3) {
        _divisors[*pos] = current;
        ++*pos;
        return;
    }
    seq_t mult = 1;
    for (int e = 0; e <= _p_exp[idx]; ++e) {
        _gen_divisors(idx + 1, current * mult, pos);
        mult *= (seq_t)_p_factors[idx];
    }
}

/* Simple insertion sort – works for all seq_t types that support < */
static void _sort_divisors(size_t count)
{
    for (size_t i = 1; i < count; ++i) {
        seq_t key = _divisors[i];
        size_t j = i;
        while (j > 0 && key < _divisors[j - 1]) {
            _divisors[j] = _divisors[j - 1];
            --j;
        }
        _divisors[j] = key;
    }
}

/* One‑time initialisation routine */
static void _init_divisors(void)
{
    if (_init_done) return;
    size_t cnt = 0;
    _gen_divisors(0, (seq_t)1, &cnt);    /* cnt will be 12 */
    _sort_divisors(cnt);
    _init_done = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    _init_divisors();
    if (n <= 12) return _divisors[n - 1];
    return (seq_t)0;   /* Undefined beyond the 12 divisors */
}
