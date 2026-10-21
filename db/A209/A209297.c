/*
 * A209297  Triangle read by rows: T(n,k) = k*n + k - n, 1 <= k <= n.
 * https://oeis.org/A209297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = k*(r+1) - r, where r is the smallest integer with *
 *            r*(r+1)/2 ≥ n and k = n - (r-1)*r/2. This implements the
 *            triangle * T(n,k) = k*n + k - n read by rows
 * Range:     u8 n <= 135, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


static unsigned long long isqrt128(unsigned __int128 x) {
    unsigned long long low = 0;
    unsigned long long high = (unsigned long long)-1; /* inclusive */
    unsigned long long ans = 0;
    while (low <= high) {
        unsigned long long mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* safety; a(0) is undefined */

    unsigned __int128 t = (unsigned __int128)8 * n + 1;
    unsigned long long sqrt_t = isqrt128(t);
    unsigned long long r = (sqrt_t - 1) >> 1;          /* floor value */

    unsigned __int128 cum = (unsigned __int128)r * (r + 1) / 2;
    if (cum < n) r++;                                 /* adjust to ceil */

    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    unsigned long long k = n - (unsigned long long)prev;

    unsigned __int128 val = (unsigned __int128)k * (r + 1) - r;
    return (seq_t)val;
}
