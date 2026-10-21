/*
 * A216230  Square array T, read by antidiagonals: T(n,k) = 0 if n-k>=1 or if
 *          k-n>=2, T(0,0) = T(0,1) = 1, T(n,k) = T(n-1,k) + T(n,k-1).
 * https://oeis.org/A216230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if n is of the form * n = s*(s+1)/2 + floor(s/2) for
 *            some integer s ≥ 0, * else a(n) = 0. * Here s is the largest
 *            integer with s*(s+1)/2 ≤ n
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


static uint64_t find_s(idx_t n)
{
    uint64_t low = 0;
    uint64_t high = n;   /* safe upper bound – high ≤ 2^64‑1 */
    while (low < high) {
        uint64_t mid = low + ((high - low + 1) >> 1);
        unsigned __int128 tri =
            (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= (unsigned __int128)n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    uint64_t s = find_s(n);
    unsigned __int128 tri128 = (unsigned __int128)s * (s + 1) / 2;
    uint64_t tri = (uint64_t)tri128;          /* tri ≤ n, fits in 64 bits */
    uint64_t r = n - tri;                    /* position in diagonal s */
    return (r == s / 2) ? (seq_t)1 : (seq_t)0;
}
