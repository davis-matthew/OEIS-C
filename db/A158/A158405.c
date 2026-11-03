/*
 * A158405  Triangle T(n,m) = 1+2*m of odd numbers read along rows, 0<=m<n.
 * https://oeis.org/A158405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2*i-1, where i = n - t(t+1)/2, t = floor( (-1 +
 *            sqrt(8*n-7)) / 2 )
 * Range:     u8 n <= 8384, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Validated: standard, pass, 94 terms through n = 9847 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t _triangle_probe(idx_t n) {
    idx_t low  = 0;
    idx_t high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;                 /* upper mid to avoid infinite loop */
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if ((idx_t)tri < n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n) {
    idx_t t  = _triangle_probe(n);
    idx_t tri = (idx_t)((unsigned __int128)t * (t + 1) / 2);
    idx_t i   = n - tri;
    seq_t res = ((seq_t)2 * (seq_t)i) - (seq_t)1;
    return res;
}
