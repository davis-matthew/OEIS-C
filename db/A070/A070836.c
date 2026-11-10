/*
 * A070836  Duplicate of A063432.
 * https://oeis.org/A070836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = a(n) of A063432: triangle read by rows, entry k in row r
 *            is r written in base k (k=1: r ones), digits concatenated in
 *            decimal
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 19, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A063432_pow10_of_uint64(uint64_t d)
{
    seq_t p = 1;
    if (d == 0) return (seq_t)10;
    while (d)
    {
        p *= (seq_t)10;
        d /= 10;
    }
    return p;
}

static seq_t A063432_a(idx_t n)
{
    if (n == 0) return 0;

    idx_t lo = 1, hi = 1;
    while (hi < n && (unsigned __int128)hi * (hi + 1) / 2 < n)
        hi *= 2;
    if (hi > n) hi = n;
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    idx_t offset = n - (idx_t)((unsigned __int128)(r - 1) * r / 2);
    idx_t k = offset;

    if (k == 1)
    {
        seq_t val = 0;
        for (idx_t i = 0; i < r; ++i)
            val = val * (seq_t)10 + (seq_t)1;
        return val;
    }

    unsigned int digits[128];
    unsigned int len = 0;
    idx_t temp = r;
    while (temp > 0)
    {
        digits[len++] = (unsigned int)(temp % k);
        temp /= k;
    }

    seq_t val = 0;
    for (int i = (int)len - 1; i >= 0; --i)
    {
        seq_t digit = digits[i];
        seq_t p = A063432_pow10_of_uint64(digits[i]);
        val = val * p + digit;
    }
    return val;
}

seq_t a(idx_t n)
{
    return A063432_a(n);
}
