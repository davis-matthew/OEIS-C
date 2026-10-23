/*
 * A036488  Nearest integer to n^(5/2).
 * https://oeis.org/A036488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to n^(5/2)
 * Range:     u8 n <= 9, u16 n <= 84, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square‑root of an unsigned __int128 value */
static inline unsigned __int128 floor_sqrt_uint128(unsigned __int128 x)
{
    if (x == 0) return 0;
    unsigned __int128 low = 0, high = 1;
    while (high * high <= x) high <<= 1;           /* find an upper bound      */
    while (high - low > 1) {                       /* binary search            */
        unsigned __int128 mid = (low + high) >> 1;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* a(n) = nearest integer to n^(5/2) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* compute n^5 as an unsigned __int128 */
    unsigned __int128 x = (unsigned __int128)n;
    x *= n;          /* n^2 */
    x *= n;          /* n^3 */
    x *= n;          /* n^4 */
    x *= n;          /* n^5 */

    unsigned __int128 m = floor_sqrt_uint128(x);          /* m = ⌊√(n^5)⌋ */
    unsigned __int128 r = x - m * m;                      /* remainder from square */

    /* round to nearest integer: if 2r ≥ 2m+1 then round up */
    if ((r << 1) >= ((m << 1) + 1))
        ++m;

    return (seq_t)m;
}
