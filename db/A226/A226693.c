/*
 * A226693  Squarefree parts of A079896(n), n>= 1.
 * https://oeis.org/A226693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = squarefree part of D(n), where D(n) is the nth positive
 *            integer that is 0 or 1 mod 4 and not a perfect square
 * Range:     u8 n <= 24, u16 n <= 7242, u32 n <= 20001, u64 n <= 20001,
 *            u128 n <= 20001, big any n
 * Validated: standard, pass, 84 terms through n = 9649 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static seq_t squarefree_part(seq_t x)
{
    seq_t result = 1;
    seq_t temp   = x;
    for (seq_t p = 2; p <= temp / p; ++p)
    {
        int exp = 0;
        while (temp % p == 0)
        {
            temp /= p;
            ++exp;
        }
        if (exp & 1)
            result *= p;
    }
    if (temp > 1)
        result *= temp;
    return result;
}

seq_t a(idx_t n)
{
    seq_t d     = 1;
    idx_t count = 0;

    while (count < n)
    {
        ++d;
        seq_t mod4 = d & 3;
        if (mod4 != 0 && mod4 != 1)
            continue;
        seq_t s = floor_sqrt(d);
        if (s * s == d)
            continue;
        ++count;
    }

    return squarefree_part(d);
}
