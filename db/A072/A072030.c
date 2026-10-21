/*
 * A072030  Array read by antidiagonals: T(n,k) = number of steps in simple
 *          Euclidean algorithm for gcd(n,k) where n >= 1, k >= 1.
 * https://oeis.org/A072030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = number of subtraction steps in the simple Euclidean
 *            algorithm for gcd(r,c), where (r,c) is the pair at index n in
 *            the antidiagonal enumeration of positive integer pairs. In other
 *            words, let L be the smallest integer with L*(L-1)/2 ≥ n. Then r
 *            = n − (L-1)*(L-2)/2, c = L − r, and a(n) = T(r,...
 * Range:     u8 n <= 1225, u16 n <= 1225, u32 n <= 1225, u64 n <= 1225,
 *            u128 n <= 1225, big any n
 * Helpers:   sub_steps
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sub_steps.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* offset is 1, but guard */

    /* find minimal L such that L*(L-1)/2 >= n   (L >= 2) */
    idx_t low  = 2;
    idx_t high = n + 1;          /* high is surely large enough */
    while (low < high) {
        idx_t mid   = low + (high - low) / 2;
        __int128 cnt = (__int128)mid * (mid - 1) / 2;
        if (cnt >= n) high = mid;
        else          low  = mid + 1;
    }
    idx_t L = low;

    /* previous count of elements before diagonal L */
    __int128 prev_cnt = (__int128)(L - 1) * (L - 2) / 2;
    idx_t prev = (idx_t)prev_cnt;

    /* coordinates on the diagonal */
    idx_t r = n - prev;          /* 1 <= r <= L-1 */
    idx_t c = L - r;             /* 1 <= c <= L-1 */

    /* compute the number of subtraction steps */
    seq_t a_val = r > c ? r : c;
    seq_t b_val = r > c ? c : r;
    return sub_steps(a_val, b_val);
}
