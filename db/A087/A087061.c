/*
 * A087061  Array A(n, k) = lunar sum n + k (n >= 0, k >= 0) read by
 *          antidiagonals.
 * https://oeis.org/A087061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, nice, base, look
 * Formula:   a(n) = lunar addition A(i,j) where (i,j) is the n‑th position in
 *            the infinite array read by antidiagonals. A(i,j) is defined by
 *            A(i,j) = max(i,j) if i<10 && j<10 A(i,j) = 10*A(i/10,j/10) +
 *            A(i%10,j%10) otherwise
 * Range:     u8 n <= 10010, u16 n <= 10010, u32 n <= 10010, u64 n <= 10010,
 *            u128 n <= 10010, big any n
 * Validated: standard, pass, 113 terms through n = 9548 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Lunar addition of two non‑negative integers. */
static seq_t lunar_add(idx_t a, idx_t b)
{
    if (a < 10 && b < 10)
        return (seq_t)(a > b ? a : b);
    return (seq_t)(lunar_add(a / 10, b / 10) * 10 + lunar_add(a % 10, b % 10));
}

/* Integer square root for 0 ≤ x < 2⁶⁴  (uses division to avoid overflow). */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0;
    uint64_t hi = 4294967296ULL;            /* 2³², > sqrt(2⁶⁴)      */
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (mid > 0 && mid > x / mid)      /* mid*mid > x (overflow safe) */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Return the a(n) term of A087061. */
seq_t a(idx_t n)
{
    /* Find the diagonal k such that k*(k+1)/2 ≤ n < (k+1)*(k+2)/2. */
    /* Diagonal index:  k = floor((sqrt(8*n+1)-1)/2)  */
    uint64_t tmp = (uint64_t)(((__uint128_t)8 * n) + 1);   /* 8*n+1 may need 128‑bit */
    uint64_t sqrtval = isqrt_u64(tmp);
    uint64_t k = (sqrtval - 1) / 2;

    uint64_t prev = k * (k + 1) / 2;        /* number of terms before diagonal k */
    uint64_t t = n - prev;                 /* offset inside the diagonal */

    idx_t i = k - t;
    idx_t j = t;

    return lunar_add(i, j);
}
