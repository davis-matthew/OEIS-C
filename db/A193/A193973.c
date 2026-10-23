/*
 * A193973  Triangular array: the fission of (p(n,x)) by (q(n,x)), where
 *          p(n,x)=x*p(n-1,x)+n+1 with p(0,x)=1, and q(n,x)=x*p(n-1,x)+1 with
 *          p(0,x)=1.
 * https://oeis.org/A193973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(n+2) – T(n+1–k), where r = floor((sqrt(8n+1)–1)/2) (row
 *            index), k = n – r*(r+1)/2 (column index)
 * Range:     u8 n <= 247, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;                 /* r cannot exceed n */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Locate the row and column of the flattened array. */
    idx_t r = find_row(n);
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;             /* 0 <= k <= r */

    /* Compute the two triangular numbers in 128‑bit integer
       to avoid overflow before casting to seq_t. */
    __uint128_t t1 = (__uint128_t)(r + 2) * (r + 3) / 2;
    idx_t idx_out = (r + 1) - k;       /* n+1-k  */
    __uint128_t t2 = (__uint128_t)idx_out * (idx_out + 1) / 2;

    return (seq_t)(t1 - t2);
}
