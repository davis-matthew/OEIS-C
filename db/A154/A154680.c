/*
 * A154680  Triangle read by rows where T(m,n)=2*m*n + m + n - 2.
 * https://oeis.org/A154680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2*m*k + m + k - 2 * where m is the smallest integer such
 *            that m*(m+1)/2 ≥ n * (m = ceil((sqrt(8*n + 1) - 1)/2)), and k =
 *            n - m*(m-1)/2
 * Range:     u8 n <= 65, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t floor_sqrt128(__int128 x)
{
    /* integer square‑root of a non‑negative 128‑bit number
       returns floor(sqrt(x)) */
    unsigned long long lo = 0;
    unsigned long long hi = 1;
    while ((unsigned __int128)hi * hi <= (unsigned __int128)x) {
        hi <<= 1;
    }
    lo = hi >> 1;
    while (lo < hi) {
        unsigned long long mid = lo + ((hi - lo + 1) >> 1);
        if ((unsigned __int128)mid * mid <= (unsigned __int128)x) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offsets < 1 but safe */

    /* find m such that m*(m+1)/2 ≥ n */
    __int128 s = (__int128)8 * n + 1;
    uint64_t r = floor_sqrt128(s);          /* floor(sqrt(8*n+1))          */
    uint64_t m = (r - 1) / 2;               /* preliminary candidate       */
    /* adjust m to the minimal value satisfying the triangular bound */
    while (( (__int128)m * (m + 1) ) / 2 < n) m++;

    /* compute k = n - T_{m-1} where T_{m-1} = m*(m-1)/2 */
    uint64_t t_prev = (uint64_t)(( (__int128)m * (m - 1) ) / 2);
    uint64_t k = n - t_prev;

    /* final formula using 128‑bit arithmetic to avoid overflow */
    unsigned __int128 val128 = (unsigned __int128)2 * m * k + m + k - 2;
    return (seq_t)val128;
}
