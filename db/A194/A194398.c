/*
 * A194398  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) < 0, where
 *          r=sqrt(15) and < > denotes fractional part.
 * https://oeis.org/A194398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A194398: numbers m such that Sum_{k=1..m}
   (<1/2 + k*sqrt(15)> - <k*sqrt(15)>) < 0
   Computation: iterate m, count how many k ≤ m have
   fractional part {k*sqrt(15)} < 1/2.
   m belongs to the sequence iff 2·lowcnt < m. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* integer floor of sqrt(x) for a 64‑bit unsigned integer */
static uint64_t _isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 32);          /* hi^2 ≥ UINT64_MAX */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid > x / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* true if the fractional part of k*sqrt(15) is < 1/2 */
static bool _is_low(idx_t k)
{
    /* 15*k^2 fits in 128 bits for all k processed here */
    __int128 L128 = (__int128)15 * k * k;
    uint64_t L = (uint64_t)L128;                /* <= UINT64_MAX */

    uint64_t a = _isqrt_u64(L);                 /* floor(sqrt(L)) */

    /* inequality 4*L <= 4*a^2 + 4*a  ⇔ {k*sqrt(15)} < 1/2  */
    __int128 lhs = (__int128)4 * L;
    __int128 rhs = (__int128)4 * a * a + (__int128)4 * a;

    return lhs <= rhs;
}

/* cached terms of the sequence */
static seq_t * _terms    = NULL;
static size_t _terms_cap = 0;
static size_t _terms_len = 0;

/* runtime state */
static uint64_t _low_cnt = 0;   /* count of low fractional parts up to current m */
static idx_t _cur_m = 0;        /* last processed m */

/* return the n‑th term of the sequence (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but keep ABI stable */

    while (_terms_len < (size_t)n) {
        ++_cur_m;

        if (_is_low(_cur_m))
            ++_low_cnt;

        /* m is a term iff 2*low_cnt < m */
        if ((uint64_t)2 * _low_cnt < _cur_m) {
            if (_terms_len == _terms_cap) {
                size_t new_cap = (_terms_cap == 0) ? 8 : _terms_cap << 1;
                seq_t *tmp = seq_alloc_array(new_cap);
                for (size_t i = 0; i < _terms_len; ++i)
                    tmp[i] = _terms[i];
                if (_terms) seq_free_array(_terms);
                _terms = tmp;
                _terms_cap = new_cap;
            }
            _terms[_terms_len++] = (seq_t)_cur_m;
        }
    }

    return _terms[n - 1];
}
