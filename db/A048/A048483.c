/*
 * A048483  Array read by antidiagonals: T(k,n) = (k+1)2^n - k.
 * https://oeis.org/A048483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (k+1)·2^m – k where k = n – s·(s+1)/2, m = s – k, s =
 *            floor( (sqrt(8n+1) – 1)/2 ) (s is the anti‑diagonal index)
 * Range:     u8 n <= 35, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find s such that s*(s+1)/2 ≤ n < (s+1)*(s+2)/2
       via binary search – works for any n ≥ 0 */
    idx_t lo = 0;
    idx_t hi = n;                    /* s can never exceed n */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        __int128 prod = (__int128)mid * (mid + 1);   /* mid*(mid+1) */
        if (prod <= (__int128)n * 2)        /* prod/2 ≤ n  */
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t s = lo;

    /* k = n minus the number of terms before this diagonal */
    __int128 prev128 = (__int128)s * (s + 1) / 2;
    idx_t prev = (idx_t)prev128;           /* safe: ≤ 9·10¹⁸ for 64‑bit n */
    idx_t k   = n - prev;                  /* 0 ≤ k ≤ s */
    idx_t m   = s - k;                     /* 0 ≤ m ≤ s */

    seq_t term = (seq_t)(k + 1) * pow2(m) - (seq_t)k;
    return term;
}
