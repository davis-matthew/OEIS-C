/*
 * A127774  Triangle read by rows: row n consists of n-1 zeros followed by
 *          A000292(n).
 * https://oeis.org/A127774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 0 unless n is a triangular number T(k)=k*(k+1)/2; then
 *            a(n) = C(k+2,3) = k*(k+1)*(k+2)/6
 * Range:     u8 n <= 65, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned __int128 value */
static inline idx_t isqrt_u128(unsigned __int128 v)
{
    idx_t low = 0, high = 1;
    /* Find an upper bound for the root */
    while ((unsigned __int128)high * high <= v)
        high <<= 1;
    /* Binary search in [low, high] */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * mid <= v)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* state before the offset */

    /* compute m such that m*(m+1)/2 = n ? */
    unsigned __int128 val = (unsigned __int128)1 + (unsigned __int128)8 * n;
    idx_t s = isqrt_u128(val);
    idx_t m = (s - 1) / 2;

    /* check if n is triangular */
    seq_t t = (seq_t)m * (seq_t)(m + 1) / 2;
    if (t != n)
        return 0;   /* not a triangular index */

    /* return the tetrahedral number C(m+2,3) */
    return (seq_t)m * (seq_t)(m + 1) * (seq_t)(m + 2) / 6;
}
