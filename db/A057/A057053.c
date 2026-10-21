/*
 * A057053  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          each k is an R(i(k),j(k)) and A057053(n)=j(n^3).
 * https://oeis.org/A057053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = j(n^3), where j(k) is the column index of the number k in
 *            the anti‑diagonal enumeration of the infinite rectangle R(i,j):
 *            1; 2,3; 4,5,6; 7,8,9,10; ... That is, • let d be the smallest
 *            integer with d(d+1)/2 ≥ k; • t = (d−1)d/2 (total numbers before
 *            diagonal d); • pos = k−t (1–based position...
 * Range:     u8 n <= 39, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute k = n^3  in 128‑bit arithmetic */
    unsigned __int128 n128 = (unsigned __int128)n;
    unsigned __int128 k = n128 * n128 * n128;        /* k = n^3 */

    /* binary search for d: minimal d with d(d+1)/2 >= k  */
    unsigned long long low = 1, high = 1;
    while ( ( (__int128)high * (high + 1) ) / 2 < k )
        high <<= 1;                      /* double until upper bound */

    while (low < high) {
        unsigned long long mid = low + (high - low) / 2;
        if ( ( (__int128)mid * (mid + 1) ) / 2 >= k )
            high = mid;
        else
            low = mid + 1;
    }
    unsigned long long d = low;           /* diagonal index */

    /* t = (d-1)d/2 */
    unsigned __int128 t = (unsigned __int128)(d - 1) * d / 2;

    /* pos = k - t  (1‑based ) */
    unsigned __int128 pos128 = k - t;
    unsigned long long pos = (unsigned long long)pos128; /* fits within d */

    /* j = d - pos + 1 */
    unsigned long long j = d - pos + 1;

    return (seq_t)j;
}
