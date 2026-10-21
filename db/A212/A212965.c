/*
 * A212965  Number of triples (w,x,y) with all terms in {0,...,n} and such
 *          that w = max(w,x,y) - min(w,x,y).
 * https://oeis.org/A212965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*n*(n+1) + (2*n+1)*(-1)^n + 7)/8
 * Range:     u8 n <= 11, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a signed 128‑bit integer for safe intermediate arithmetic.
       The final result fits the target type whenever it fits. */
    signed __int128 N = n;
    signed __int128 numerator = 14 * N * (N + 1) + 7;
    if ((n & 1) == 0)
        numerator += (2 * N + 1);       /* (-1)^n = +1  when n is even */
    else
        numerator -= (2 * N + 1);       /* (-1)^n = -1  when n is odd */

    numerator /= 8;                      /* division is exact */
    return (seq_t)numerator;
}
