/*
 * A092137  Lower bound for A005842(n).
 * https://oeis.org/A092137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil( sqrt( sum_{k=1}^n k^2 ) )
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of squares: S(n) = n(n+1)(2n+1)/6.  Ceil(sqrt(S(n))) is returned. */

#include <stdint.h>  /* for uint64_t used by idx_t */

static inline unsigned __int128 isqrt128(unsigned __int128 x)
{
    /* floor of sqrt(x) for unsigned 128-bit integer */
    unsigned __int128 lo = 0, hi = (unsigned __int128)1 << 64; /* 2^64 > sqrt(max unsigned __int128) */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        if (mid * mid > x)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* we return 0 for safety when n < 1 -- offset is 1 */

    /* Use unsigned __int128 for all intermediate arithmetic to avoid overflow for reasonable n. */
    unsigned __int128 nn = n;
    unsigned __int128 sum = nn * (nn + 1) * (2 * nn + 1) / 6;

    unsigned __int128 r = isqrt128(sum);        /* floor sqrt */

    /* Ceil sqrt: if r^2 < sum, use r+1 else r */
    if (r * r < sum) r += 1;

    /* Cast the result back to seq_t.  For small seq_t types the value may truncate
       but that is acceptable because the sequence is only required to be correct
       in the range that fits the type. */
    return (seq_t)r;
}
