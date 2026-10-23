/*
 * A176286  Triangle T(n,k) = 1 + 2*k*(n-k)*(k^2 -n*k +2*n^2) read by rows.
 * https://oeis.org/A176286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = 1 + 2*k*(n-k)*(k^2 - n*k + 2*n^2) * a(n) is obtained by
 *            reading T row‑wise: * a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1),
 *            a(3)=T(2,0), … * This implementation works for any n >= 0 and
 *            uses only integer arithmetic
 * Range:     u8 n <= 15, u16 n <= 158, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned __int128 values */
static idx_t isqrt_u128(unsigned __int128 n)
{
    idx_t lo = 0, hi = (idx_t)1ull << 31;            /* hi^2 > n for all 128‑bit n */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Sequence A176286 */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    unsigned __int128 s = (unsigned __int128)n * 8 + 1;          /* 8*n + 1 */
    idx_t sqrt_val = isqrt_u128(s);
    idx_t r = (sqrt_val - 1) >> 1;                               /* floor((sqrt(8n+1)-1)/2) */

    /* Start index of this row */
    unsigned __int128 start = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = (idx_t)(n - start);                               /* position in row */

    /* Compute T(r,k) using 128‑bit arithmetic to avoid overflow */
    unsigned __int128 n128 = (unsigned __int128)r;
    unsigned __int128 k128 = (unsigned __int128)k;

    unsigned __int128 term = (unsigned __int128)1
        + (unsigned __int128)2 * k128 * (n128 - k128)
        * (k128 * k128 - n128 * k128 + (unsigned __int128)2 * n128 * n128);

    return (seq_t)term;
}
