/*
 * A189530  n+[ns/r]+[nt/r]; r=1, s=2^(1/3), t=2^(2/3).
 * https://oeis.org/A189530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n * 2^{1/3}) + floor(n * 2^{2/3})
 * Range:     u8 n <= 66, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the identity floor(n*2^{1/3}) = floor( cbrt(n^3*2) ) and
   floor(n*2^{2/3}) = floor( cbrt(n^3*4) ) so all arithmetic is
   integer‑only. */
#include <stdint.h>

/* compute the integer cube root: floor( x^{1/3} ) for x <= 2^126-1 */
static inline unsigned __int128 cube(unsigned __int128 x)
{
    return x * x * x;
}

static inline unsigned __int128 cube_root_floor(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = 1;
    /* grow upper bound until hi^3 > x */
    while (cube(hi) <= x)
        hi <<= 1;
    /* binary search in [lo,hi) */
    while (lo + 1 < hi)
    {
        unsigned __int128 mid = (lo + hi) >> 1;
        if (cube(mid) <= x)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* although the OEIS offset is 1, return 0 for safety */
        return 0;

    unsigned __int128 n128 = (unsigned __int128)n;

    /* Compute n^3 once; it fits in 128 bits for n up to at least 10^4 */
    unsigned __int128 n3 = n128 * n128 * n128;

    /* floor(n * 2^{1/3}) = floor( cbrt(n^3 * 2) ) */
    unsigned __int128 val1 = cube_root_floor(n3 * 2);

    /* floor(n * 2^{2/3}) = floor( cbrt(n^3 * 4) ) */
    unsigned __int128 val2 = cube_root_floor(n3 * 4);

    /* Combine the parts; the result comfortably fits in seq_t for expected inputs */
    seq_t result = (seq_t)n + (seq_t)val1 + (seq_t)val2;
    return result;
}
