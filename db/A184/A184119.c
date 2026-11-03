/*
 * A184119  Upper s(n)-Wythoff sequence, where s(n) = 2n - 1; complement of
 *          A136119.
 * https://oeis.org/A184119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2+sqrt(2))*n - sqrt(2)/2) (Offset 1)
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 116 terms through n = 1761 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Equivalent integer form: a(n) = 2*n + floor_sqrt(2*(2*n-1)^2) / 2   */

/* Uses the bundled integer square‑root routine. */
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

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, a(0) is undefined – return 0 safely */
        return 0;

    /* 2*n (converted to seq_t to avoid overflow in idx_t on large n) */
    seq_t two_n = (seq_t)2 * (seq_t)n;

    /* m = 2*n - 1  (computing in idx_t then casting to seq_t) */
    seq_t m = (seq_t)((idx_t)2 * n - (idx_t)1);

    /* val = 2 * m * m ; floor_sqrt(val) gives floor(sqrt(2)*m) */
    seq_t val = (seq_t)2 * m * m;
    seq_t s   = floor_sqrt(val);

    /* a(n) = 2*n + floor_sqrt(val)/2 */
    return two_n + s / (seq_t)2;
}
