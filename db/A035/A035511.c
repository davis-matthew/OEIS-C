/*
 * A035511  3rd column of Inverse Stolarsky array.
 * https://oeis.org/A035511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 12 for n > 0: a(n) = 2*round(n*phi) +
 *            3*floor((round(n*phi)+1/2)*phi) + 4, where phi = (1+sqrt(5))/2
 *            The implementation avoids any floating‑point operations
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = x;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ((__int128)mid * mid <= (__int128)x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)12;

    /* Use 64‑bit arithmetic for the intermediate products that may overflow
       a 32‑bit seq_t. */
    uint64_t n_ll = (uint64_t)n;

    /* s1 = floor(n * sqrt(5)) = floor(sqrt(5*n^2)) */
    uint64_t val1 = 5ULL * n_ll * n_ll;
    seq_t s1 = (seq_t)isqrt_u64(val1);

    /* parity of (n + s1) */
    seq_t parity = (((seq_t)n) + s1) & (seq_t)1;

    /* r = round(n*phi) */
    seq_t r = (((seq_t)n) + s1 + parity) / (seq_t)2;

    /* Compute 2*m+1 where m = r */
    seq_t two_m_plus1 = (seq_t)2 * r + (seq_t)1;

    /* s2 = floor((2*m+1)*sqrt(5)) = floor(sqrt(5*(2*m+1)^2)) */
    uint64_t val2 = 5ULL * (uint64_t)two_m_plus1 * (uint64_t)two_m_plus1;
    seq_t s2 = (seq_t)isqrt_u64(val2);

    /* p = floor((m+1/2)*phi) = ((2*m+1)+floor((2*m+1)*sqrt5))/4 */
    seq_t p = (two_m_plus1 + s2) / (seq_t)4;

    /* Final value: a(n) = 2*r + 3*p + 4 */
    return (seq_t)2 * r + (seq_t)3 * p + (seq_t)4;
}
