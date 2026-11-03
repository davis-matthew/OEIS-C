/*
 * A219227  a(n) is the sum of n addends nested as follows:
 *          floor(f(floor(f(...(n)...)))) where f(x) = x^(1/3).
 * https://oeis.org/A219227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n} f^k(n), where f(x) = ⌊x^(1/3)⌋ (the same
 *            squares floor operation is used at every step)
 * Range:     u8 n <= 63, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

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
    /* Handle the offset: a(1) is the first term. */
    if (n == 0)
        return 0;                   /* undefined for the sequence, safe fallback */

    seq_t sum = 0;
    seq_t cur = n;

    /* Iterate exactly n times, computing the nested floor cube roots. */
    for (idx_t i = 0; i < n; ++i) {
        cur = floor_cube_root(cur);  /* f(cur) */
        sum += cur;

        if (cur == 1) {
            /* Remaining terms are all 1.  Subtract the already processed term. */
            sum += (n - 1 - i);
            break;
        }
    }

    return sum;
}
