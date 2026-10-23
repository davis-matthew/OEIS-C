/*
 * A136331  The discriminant of the characteristic polynomial of the O+ and O-
 *          submatrix for spin 3 of the nuclear electric quadrupole
 *          Hamiltonian is a perfect square for these values.
 * https://oeis.org/A136331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * b(n) where b(0)=0, b(1)=1, b(2)=2, b(3)=7, b(4)=16,
 *            b(5)=65; for n>=6: b(n) = 11*(b(n-2)-b(n-4)) + b(n-6)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t* _cache = NULL;
static idx_t _cache_len = 0;
static idx_t _cache_cap = 0;

static void _ensure_capacity(idx_t needed)
{
    if (needed <= _cache_cap)
        return;
    idx_t new_cap = _cache_cap ? _cache_cap * 2 : 64;
    if (new_cap < needed)
        new_cap = needed;
    _cache = (seq_t*)realloc(_cache, (size_t)new_cap * sizeof(seq_t));
    _cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (_cache_len == 0) {
        /* initialize base values for b(n) */
        _ensure_capacity(6);
        _cache[0] = (seq_t)0;  /* b(0) */
        _cache[1] = (seq_t)1;  /* b(1) */
        _cache[2] = (seq_t)2;  /* b(2) */
        _cache[3] = (seq_t)7;  /* b(3) */
        _cache[4] = (seq_t)16; /* b(4) */
        _cache[5] = (seq_t)65; /* b(5) */
        _cache_len = 6;
    }

    if (n >= _cache_len) {
        for (idx_t i = _cache_len; i <= n; ++i) {
            _ensure_capacity(i + 1);
            seq_t diff = _cache[i - 2] - _cache[i - 4];
            seq_t val  = diff * (seq_t)11 + _cache[i - 6];
            _cache[i] = val;
        }
        _cache_len = n + 1;
    }

    /* return the desired sequence value a(n) = 3 * b(n) */
    return _cache[n] * (seq_t)3;
}
