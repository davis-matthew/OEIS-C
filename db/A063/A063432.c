/*
 * A063432  Triangle read by rows in which k-th entry in row n is
 *          representation of n in base k, for 1 <= k <= n.
 * https://oeis.org/A063432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabl
 * Formula:   a(n) is the nth entry of the triangle read by rows, where the *
 *            entry in row r, column k is the representation of r in base k. *
 *            For base 1 this is a repunit of r ones; otherwise the base‑k
 *            digits are * concatenated as decimal numbers. This function
 *            finds r and k from n * using the triangular num...
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   pow10_of_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10_of_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* unused, but keeps type safety */

    /* Find row r such that r*(r+1)/2 >= n and (r-1)*r/2 < n. */
    idx_t lo = 1, hi = 1;
    while (hi < n && (unsigned __int128)hi * (hi + 1) / 2 < n)
        hi *= 2;
    if (hi > n) hi = n;               /* r never exceeds n */
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;                     /* row number */

    /* Column k = offset within the row. */
    idx_t offset = n - (idx_t)((unsigned __int128)(r - 1) * r / 2);
    idx_t k = offset;                 /* 1 <= k <= r */

    /* Base 1: concatenate r ones. */
    if (k == 1)
    {
        seq_t val = 0;
        for (idx_t i = 0; i < r; ++i)
            val = val * (seq_t)10 + (seq_t)1;
        return val;
    }

    /* Convert r to base k. */
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
        seq_t p = pow10_of_uint64(digits[i]);   /* 10^(decimal digits of digit) */
        val = val * p + digit;
    }
    return val;
}
