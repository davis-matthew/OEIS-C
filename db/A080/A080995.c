/*
 * A080995  Characteristic function of generalized pentagonal numbers A001318.
 * https://oeis.org/A080995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is a generalized pentagonal number, i.e. if 24*n+1
 *            is a perfect square and its square root is ≡ 1 or 5 (mod 6),
 *            otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned long long isqrt128(unsigned __int128 x)
{
    unsigned long long low = 0, high = (1ULL << 33) - 1;   /* sqrt(24·2^64) < 2^33 */
    unsigned long long ans = 0, mid;

    while (low <= high) {
        mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;          /* avoid underflow */
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    unsigned __int128 D = (unsigned __int128)24 * n + 1; /* D = 24*n + 1 */
    unsigned long long s = isqrt128(D);

    if ((unsigned __int128)s * (unsigned __int128)s != D)
        return (seq_t)0;                       /* not a square -> not pentagonal */

    unsigned long long r = s % 6;
    return (seq_t)((r == 1 || r == 5) ? 1 : 0);
}
