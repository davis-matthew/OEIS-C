/*
 * A129685  Exponential Riordan array [1-x^2/2, x].
 * https://oeis.org/A129685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A129685 – Exponential Riordan array [1‑x²/2, x]
    a(0) = 1
    a(1) = 0
    a(2) = 1
    a(3) = -1
    a(4) = 0
    a(5) = 1
    ...
    a(n) =  1  if n = m*(m+3)/2  (m ≥ 0)
    a(n) = -k*(k+1)/2 if n = k*(k+5)/2  (k ≥ 0)
    a(n) =  0  otherwise
*/

#include <stdint.h>

/* Return a(n) according to the definitions above */
seq_t a(idx_t n)
{
    /* First check the positive‑one pattern */
    for (idx_t m = 0; ; ++m) {
        __int128 v = (__int128)m * (m + 3) / 2;
        if (v > n) break;
        if ((idx_t)v == n) return (seq_t)1;
    }

    /* Then check the negative‑triangular pattern */
    for (idx_t k = 0; ; ++k) {
        __int128 v = (__int128)k * (k + 5) / 2;
        if (v > n) break;
        if ((idx_t)v == n) {
            seq_t kk = (seq_t)k;
            return -(kk * (kk + 1) / 2);
        }
    }

    /* All other indices give 0 */
    return (seq_t)0;
}
