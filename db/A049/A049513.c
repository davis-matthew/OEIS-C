/*
 * A049513  Array T by antidiagonals: T(k,n) = k*n*2^(n-1) + 1, n >= 0, k >=
 *          0.
 * https://oeis.org/A049513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(k,n') where k = s - n', n' = position in antidiagonal;
 *            s is the unique integer with (s+1)(s+2)/2 > n
 * Range:     u8 n <= 41, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;

    idx_t s = 0;
    idx_t sum = 0;
    while (1) {
        sum += s + 1;       /* length of current antidiagonal */
        if (sum > n) break;
        s++;
    }
    idx_t prev_sum = sum - (s + 1);   /* cumulative size before this antidiagonal */
    idx_t pos = n - prev_sum;         /* position within the antidiagonal [0..s] */
    idx_t k   = s - pos;              /* row index */

    if (pos == 0) return 1;           /* T(k,0) is always 1 */

    seq_t val = (seq_t)k * (seq_t)pos * pow2((idx_t)pos - 1) + 1;
    return val;
}
