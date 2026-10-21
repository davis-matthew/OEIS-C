/*
 * A056943  Unused area of rectangle needed to enclose a non-touching spiral
 *          of length n on a square lattice.
 * https://oeis.org/A056943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((sqrt(8*n+1)-1)/2) * ceil((sqrt(8*n+1)-1)/2) - n
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;

    /* Find an upper bound for t such that t*(t+1)/2 > n */
    while (( (__int128)high * (high + 1) / 2 ) <= n)
    {
        if (high == (idx_t)-1)   /* prevent overflow on very large values */
            break;
        high <<= 1;
    }

    /* Binary search for the largest t with t*(t+1)/2 <= n */
    while (low < high)
    {
        idx_t mid = low + (high - low + 1) / 2;
        __int128 tri_mid = (__int128)mid * (mid + 1) / 2;
        if (tri_mid <= n)
            low = mid;
        else
            high = mid - 1;
    }

    idx_t t = low;
    __int128 tri = (__int128)t * (t + 1) / 2;

    if ((idx_t)tri == n)                 /* n is a triangular number */
        return (seq_t)((__int128)t * (t - 1) / 2);
    else                                 /*  n = tri + k  with 0 < k <= t   */
        return (seq_t)((__int128)2 * tri - n);
}
