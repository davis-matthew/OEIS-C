/*
 * A054238  Array read by downward antidiagonals: T(i,j) = bits of binary
 *          expansion of i interleaved with that of j.
 * https://oeis.org/A054238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base, tabl
 * Formula:   a(n) = T(i, j) where i = n - m*(m+1)/2, j = m - i and m =
 *            floor((sqrt(8*n+1)-1)/2) is the antidiagonal number. T(i, j)
 *            interleaves the binary bits of j into the even positions and the
 *            bits of i into the odd positions of the result
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9593 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Interleave bits of a (odd positions) and b (even positions). */
static inline seq_t interleave_bits(idx_t a, idx_t b)
{
    seq_t res = 0;
    idx_t pos   = 0;          /* position in result   */
    /* Extract bits from LSB upwards. */
    while (a | b)
    {
        if (b & 1) res |= ((seq_t)1) << pos;   /* even position */
        ++pos;
        if (a & 1) res |= ((seq_t)1) << pos;   /* odd  position */
        ++pos;
        a >>= 1;
        b >>= 1;
    }
    return res;
}

/* Find the largest m such that m*(m+1)/2 <= n. */
static inline idx_t max_m(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;
    /* Grow high until the condition fails. */
    while (((unsigned __int128)high * (high + 1)) / 2 <= n)
    {
        if ((high & (high - 1)) == 0)   /* check for overflow */
        {
            if (high > (UINT64_MAX >> 1))
            {
                high = n;
                break;
            }
        }
        high <<= 1;
    }
    /* Binary search between low and high. */
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;   /* largest m with m*(m+1)/2 <= n */
}

seq_t a(idx_t n)
{
    /* Locate antidiagonal m. */
    idx_t m = max_m(n);

    /* Offset of first element of diagonal m. */
    unsigned __int128 off128 = (unsigned __int128)m * (m + 1) / 2;
    idx_t     offset       = (idx_t)off128;          /* fits because off128 <= n */

    /* Position within the diagonal. */
    idx_t i = n - offset;         /* 0 <= i <= m */
    idx_t j = m - i;

    return interleave_bits(i, j);
}
