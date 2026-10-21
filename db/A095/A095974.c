/*
 * A095974  Evaluate n^4 - 93n^3 + 3196n^2 - 48008n + 265483 for n >= 0,
 *          record the primes.
 * https://oeis.org/A095974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n^4 - 93*n^3 + 3196*n^2 - 48008*n + 265483
 * Range:     i8 none, i16 none, i32 n <= 33, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 64‑bit intermediate arithmetic: n ≤ 10000 gives values ≤ 10^16 */
    int64_t nn   = (int64_t)n;
    int64_t n2   = nn * nn;           /* n^2  */
    int64_t n3   = n2 * nn;           /* n^3  */
    int64_t n4   = n3 * nn;           /* n^4  */

    int64_t val = n4 - 93 * n3 + 3196 * n2 - 48008 * nn + 265483;
    return (seq_t)val;
}
