/*
 * A184110  n + floor(3*sqrt(n+1)).
 * https://oeis.org/A184110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(3*sqrt(n+1))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t floor_sqrt_u64(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high)
    {
        idx_t mid = (low + high) >> 1;
        if (mid == 0)
        {
            low = 1;
            continue;
        }
        if (mid <= x / mid)
        {
            idx_t nxt = mid + 1;
            if (nxt > x / nxt)
                return mid;
            low = nxt;
        }
        else
            high = mid - 1;
    }
    return 0;
}

seq_t a(idx_t n)
{
    idx_t v = n + 1;                 /* n+1 */
    idx_t s = floor_sqrt_u64(v);     /* floor(sqrt(n+1)) */
    idx_t t = 3 * s;                 /* 3*floor(sqrt(n+1)) */

    /* Determine floor(3*sqrt(n+1)) accurately */
    unsigned __int128 sq = (unsigned __int128)9 * v;            /* 9*(n+1) */
    unsigned __int128 t1 = (unsigned __int128)(t + 1) * (t + 1);
    unsigned __int128 t2 = (unsigned __int128)(t + 2) * (t + 2);

    idx_t k = 0;
    if (t1 <= sq) k++;
    if (t2 <= sq) k++;

    seq_t add = (seq_t)(t + k);       /* floor(3*sqrt(n+1)) */
    return (seq_t)n + add;
}
