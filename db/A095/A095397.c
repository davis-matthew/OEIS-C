/*
 * A095397  Modified juggler map: see A095396. Largest value in trajectory of
 *          started n under the juggler map of A095396.
 * https://oeis.org/A095397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 24, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   ipow, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Modified juggler map:
   a(n) = largest value in the trajectory of started n under the
   juggler map of A095396: odd → floor(sqrt(n^3)), even → floor((n^2)^(1/3)). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
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

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    seq_t max = cur;

    while (cur != (seq_t)1)
    {
        seq_t next;
        if ((cur & (seq_t)1) == (seq_t)0)      /* even */
        {
            seq_t sq = ipow(cur, (seq_t)2);
            next = floor_cube_root(sq);
        }
        else                                   /* odd */
        {
            seq_t cube = ipow(cur, (seq_t)3);
            next = floor_sqrt(cube);
        }
        cur = next;
        if (cur > max)
            max = cur;
    }
    return max;
}
