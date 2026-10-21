/*
 * A057058  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          each k is an R(i(k),j(k)) and a(n)=i(A057027(n)).
 * https://oeis.org/A057058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = i, where i and j are the row‑index and column‑index of
 *            the unique pair (i, j) that satisfies n = t(t+1)/2 + i and j =
 *            (t²+3t+4)/2 – n; then a(n) = (i+1)/2 if i is odd, otherwise a(n)
 *            = j + i/2
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The row index t is the largest integer with t(t+1)/2 < n. */

#include <stdint.h>

/* Find the largest t such that t*(t+1)/2 < n.
   All calculations are performed with 128‑bit integers
   (via __int128) to avoid overflow. */
static inline idx_t triangular_root(idx_t n)
{
    /* exponential search to find an upper bound */
    idx_t low  = 0;
    idx_t high = 1;
    while (1) {
        __int128 tri = (__int128)high * (high + 1) / 2;
        if (tri >= (__int128)n) break;
        high <<= 1;
        /* high will never overflow for n <= 2^64-1 */
    }

    /* binary search on [low, high) for the first t with
       t(t+1)/2 >= n; the desired t is one less. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri_mid = (__int128)mid * (mid + 1) / 2;
        if (tri_mid < (__int128)n) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low - 1;   /* largest t with T_t < n */
}

seq_t a(idx_t n)
{
    /* 1. locate the row t   */
    idx_t t = triangular_root(n);

    /* 2. triangular number T_t = t*(t+1)/2   */
    __int128 tri128 = (__int128)t * (t + 1) / 2;
    idx_t t_tri = (idx_t)tri128;   /* fits into idx_t */

    /* 3. i = n - T_t   (1‑based offset inside the row) */
    idx_t i = n - t_tri;

    /* 4. j = ((t*t + 3*t + 4)/2) - n */
    __int128 num = (__int128)t * t + (__int128)3 * t + 4;
    idx_t j = (idx_t)(num / 2) - n;

    /* 5. compute a(n) */
    if (i & 1) {          /* i is odd */
        return (seq_t)((i + 1) >> 1);
    } else {              /* i is even */
        return (seq_t)(j + (i >> 1));
    }
}
