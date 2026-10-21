/*
 * A091195  Number of numbers <= n having only one abundant divisor (A091191).
 * https://oeis.org/A091195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers k ≤ n having exactly one abundant
 *            divisor. The routine precomputes which numbers up to n are
 *            abundant, then counts for each k the number of abundant divisors
 *            and accumulates a prefix sum
 * Range:     u8 n <= 5465, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t _max_n = 0;
static uint8_t  *_abundant = NULL;   /* 1 if number is abundant */
static seq_t   *_abcnt = NULL;       /* number of abundant divisors */
static seq_t   *_pref   = NULL;       /* prefix sums of a(k) */

/* Integer square‑root using binary search.  Only idx_t is needed. */
static inline idx_t _isqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Compute all tables up to at least n.  The computation is O(n √n). */
static void _ensure_up_to(idx_t n)
{
    if (n <= _max_n)
        return;   /* already computed */

    if (_abundant)   free(_abundant);
    if (_abcnt)      free(_abcnt);
    if (_pref)       free(_pref);

    /* Allocate new arrays.  calloc zero‑initialises them. */
    _abundant = (uint8_t *)calloc((size_t)(n + 1), sizeof(uint8_t));
    _abcnt    = (seq_t   *)calloc((size_t)(n + 1), sizeof(seq_t));
    _pref     = (seq_t   *)calloc((size_t)(n + 1), sizeof(seq_t));

    /* 1. Determine abundance of each number. */
    for (idx_t m = 1; m <= n; ++m) {
        uint64_t sum = 0;
        idx_t sq = _isqrt(m);
        for (idx_t d = 1; d <= sq; ++d) {
            if (m % d == 0) {
                if (d < m)
                    sum += d;
                idx_t e = m / d;
                if (e != d && e < m)
                    sum += e;
            }
        }
        _abundant[m] = (sum > m) ? 1 : 0;
    }

    /* 2. Count abundant divisors for each number. */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t count = 0;
        idx_t sq = _isqrt(m);
        for (idx_t d = 1; d <= sq; ++d) {
            if (m % d == 0) {
                if (_abundant[d])
                    ++count;
                idx_t e = m / d;
                if (e != d && _abundant[e])
                    ++count;
            }
        }
        _abcnt[m] = count;
    }

    /* 3. Build prefix sums for a(k). */
    _pref[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        _pref[i] = _pref[i - 1];
        if (_abcnt[i] == (seq_t)1)
            ++_pref[i];
    }

    _max_n = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    _ensure_up_to(n);
    return _pref[n];
}
