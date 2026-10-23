/*
 * A177708  Pentagonal triangle.
 * https://oeis.org/A177708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   sum_pent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pentagonal triangle (OEIS A177708)
   a(n) = sum_{i=b_{k-1}+1}^{b_k} P_i
   where P_i = i(3i-1)/2,
         r = ceil((sqrt(8n+1)-1)/2),
         k = n – r(r-1)/2,
         b_k = k·r – k(k-1)/2.          */

#include <stdint.h>
#include "../utils/sum_pent.h"

/* integer square root of a 64‑bit unsigned integer   */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;          /* (2^32)^2 > max u64   */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return hi;
}

/* b_k = k·r – k(k-1)/2  (k >= 0) */
static inline idx_t b_of(idx_t k, idx_t r)
{
    unsigned __int128 part1 = (unsigned __int128)k * r;
    unsigned __int128 part2 = (unsigned __int128)k * (k - 1) / 2;
    return (idx_t)(part1 - part2);
}

/* sequence entry a(n) for n >= 1   */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1; entry for 0 irrelevant */

    /* find row r such that r(r+1)/2 >= n */
    idx_t r = (isqrt_u64(8 * n + 1) - 1) / 2;
    if ((unsigned __int128)r * (r + 1) / 2 < n) r++;

    /* position k within the row (1‑based) */
    idx_t k = n - (unsigned __int128)r * (r - 1) / 2;

    /* indices of pentagonal numbers in the chunk */
    idx_t b_prev = b_of(k - 1, r);   /* k-1 is always >=0 */
    idx_t b_curr = b_of(k, r);

    return sum_pent(b_curr) - sum_pent(b_prev);
}
