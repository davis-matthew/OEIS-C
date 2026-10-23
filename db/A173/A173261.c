/*
 * A173261  Array T(n,k) read by antidiagonals: T(n,2k)=1, T(n,2k+1)=n, n>=2,
 *          k>=0.
 * https://oeis.org/A173261
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is read from the array T(n,k) by anti‑diagonals. * T(n,k)=1
 *            if k is even, T(n,k)=n if k is odd (n≥2, k≥0) * The sequence
 *            starts at n=2 (offset 2). * For an index n≥2, let m=n‑1 be the
 *            1‑based position in the * anti‑diagonal traversal. Find the
 *            unique diagonal sum s such that * (s‑1)*s/2 ≥ m....
 * Range:     u8 n <= 1327, u16 n <= 1327, u32 n <= 1327, u64 n <= 1327,
 *            u128 n <= 1327, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Below the sequence offset the result is undefined. */
    if (n < 2)
        return (seq_t)0;

    idx_t m = n - 1;                         /* 1‑based position in sequence */

    /* Find minimal s with s*(s-1)/2 >= m (s ≥ 2).  Binary search. */
    idx_t lo2 = 2, hi = 2;
    while ((__int128)hi * (hi - 1) / 2 < m) {
        hi <<= 1;                /* grow upper bound exponentially */
    }
    lo2 = (hi >> 1) + 1;        /* lower bound strictly smaller than solution */

    while (lo2 < hi) {
        idx_t mid = (lo2 + hi) >> 1;
        __int128 tri = (__int128)mid * (mid - 1) / 2;
        if (tri >= m)
            hi = mid;
        else
            lo2 = mid + 1;
    }
    idx_t s = lo2;              /* diagonal sum */

    /* Number of terms before this diagonal: (s-2)(s-1)/2 */
    __int128 prev_tri = (__int128)(s - 1) * (s - 2) / 2;
    idx_t p = m - (idx_t)prev_tri;           /* position inside diagonal (1‑based) */

    idx_t n_val = s - (p - 1);                /* row index in T */
    idx_t k     = p - 1;                      /* column index in T */

    return ((k & 1) == 0) ? (seq_t)1 : (seq_t)n_val;
}
