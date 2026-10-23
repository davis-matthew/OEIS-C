/*
 * A024548  [ 1/{n*sqrt(3)} ], where {x} := x - [ x ].
 * https://oeis.org/A024548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( 2*floor(n*sqrt(3)) / (3*n*n - floor(n*sqrt(3))^2 )
 *            )
 * Range:     u8 n <= 152, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is fully integer‐based and works for any seq_t size supported by
   the build.  It never performs floating point arithmetic and never relies on
   external libraries beyond the standard integer types.  Complexity is O(1).  */

#include <stdint.h>

/* Integer square root of an unsigned 64‑bit integer.  Result is the
   greatest r such that r*r <= x.  */
static inline idx_t isqrt_uint64(idx_t x)
{
    idx_t low = 0;
    idx_t high = (idx_t)1ULL << 32;      /* 2^32   > sqrt(UINT64_MAX) */

    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* n == 0 is out of the OEIS pre‑offset, return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* x = 3*n*n  (fits in idx_t for all n in the test range). */
    idx_t x = (idx_t)3 * n * n;

    /* m = floor(n*sqrt(3)) = floor(sqrt(3*n*n)) */
    idx_t m = isqrt_uint64(x);

    /* d = x - m*m  (>0 because 3 is square‑free). */
    idx_t d = x - m * m;

    /* a(n) = floor( (2*m)/d )  (integer division). */
    idx_t result = ((idx_t)2 * m) / d;

    return (seq_t)result;
}
