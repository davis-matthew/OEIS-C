/*
 * A182777  Beatty sequence for 3-sqrt(3).
 * https://oeis.org/A182777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(3-sqrt(3)))
 * Range:     u8 n <= 201, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline unsigned __int128 isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1 << 64) - 1;
    while (lo <= hi)
    {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 sq = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;   /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    unsigned __int128 N   = (unsigned __int128)n;
    unsigned __int128 val = 3 * N * N;
    unsigned __int128 s   = isqrt_u128(val);   /* floor(n*sqrt(3)) */
    unsigned __int128 res = 3 * N - s - 1;
    return (seq_t)res;
}
