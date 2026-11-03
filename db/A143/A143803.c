/*
 * A143803  a(n) = 2*A001614(n) - 1 where A001614 lists the Connell numbers.
 * https://oeis.org/A143803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*A001614(n) - 1 Recurrence: a(1) = 1; for n > 1, a(n) =
 *            a(n-1) + (2 if (n-1) is triangular else 4) Integer arithmetic
 *            only; uses the supplied floor_sqrt utility
 * Range:     u8 n <= 8, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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

static inline bool is_triangular(idx_t m) {
    /* A number m is triangular iff 8*m+1 is a perfect square. */
    seq_t val = ((seq_t)8 * m) + 1;
    seq_t s   = floor_sqrt(val);
    return s * s == val;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* below offset; harmless */
    seq_t result = 1;     /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        idx_t prev = i - 1;
        result += is_triangular(prev) ? 2 : 4;
    }
    return result;
}
