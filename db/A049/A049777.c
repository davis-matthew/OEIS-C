/*
 * A049777  Triangular array read by rows: T(m,n) = n + n+1 + ... + m =
 *          (m+n)(m-n+1)/2.
 * https://oeis.org/A049777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,n) = (m+n)(m-n+1)/2, where m is the row index such
 *            that m(m+1)/2 ≥ n and n = position in that row (1‑based)
 * Range:     u8 n <= 253, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A049777 (row‑by‑row triangular sums). */
#include <stdint.h>  /* for standard integer types */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for n==0 return 0 (unused). */
    if (n == 0) return 0;

    /* Find the row m such that m*(m+1)/2 >= n using binary search. */
    idx_t low  = 1;
    idx_t high = n;          /* safe upper bound: triangular(n) > n for n>=1 */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* Position j within the row (1-based). */
    __int128 tri_prev = (__int128)(m - 1) * m / 2;
    idx_t j = n - (idx_t)tri_prev;   /* 1 <= j <= m */

    /* Compute T(m,j) = (m+j)(m-j+1)/2 with 128‑bit intermediate for safety. */
    __int128 sum1 = (__int128)m + j;
    __int128 sum2 = (__int128)m - j + 1;
    __int128 prod  = sum1 * sum2;
    seq_t result   = (seq_t)(prod / 2);

    return result;
}
