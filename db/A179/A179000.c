/*
 * A179000  Array T(n,k) read by antidiagonals: coefficient [x^k] of (1 +
 *          n*Sum_{i>=1} x^i)^2, k >= 0.
 * https://oeis.org/A179000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   ceil_sqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A179000 – read antidiagonally from the array T(n,k),
   where T(n,0)=1 and T(n,k)=n*(2+n*(k-1)) for k>0.
   The linear index n (starting at 1) yields the pair
   (row=row, col=col) on the anti‑diagonal and returns T(row,col). */

#include <stdint.h>
#include "../utils/ceil_sqrt_u128.h"

seq_t a(idx_t n) {
    if (n == 0)  /* index <1 – undefined, return 0 */
        return (seq_t)0;

    /* find diagonal s: smallest s with (s+1)(s+2)/2 >= n   */
    __uint128_t v = (__uint128_t)1 + (__uint128_t)8 * n;
    idx_t ceil_sqrt = ceil_sqrt_u128(v);
    idx_t s = (ceil_sqrt - 2) / 2;

    /* number of elements before diagonal s */
    __uint128_t before = (__uint128_t)s * (s + 1) / 2;
    idx_t d = (idx_t)(n - 1 - (idx_t)before);   /* 0 <= d <= s */

    /* mapping to (row, col) */
    idx_t row = s - d + 1;   /* n-row index (1‑based) */
    idx_t col = d;           /* column index (0‑based) */

    if (col == 0)
        return (seq_t)1;
    else {
        seq_t nseq = (seq_t)row;
        seq_t cseq = (seq_t)col;
        return nseq * ((seq_t)2 + nseq * (cseq - (seq_t)1));
    }
}
