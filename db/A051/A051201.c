/*
 * A051201  Sum of elements of the set { [ n/k ] : 1 <= k <= n }.
 * https://oeis.org/A051201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{i=1}^{⌊√n⌋} ⌊n/i⌋ + ⌊n/(⌊√n⌋+1)⌋·(⌊n/(⌊√n⌋+1)⌋+1)/2
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9602 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for offset */
    idx_t m = isqrt(n);             /* floor(sqrt(n)) */
    seq_t sum = 0;

    for (idx_t i = 1; i <= m; ++i) {
        sum += (seq_t)(n / i);
    }

    idx_t t   = n / (m + 1);               /* floor(n/(m+1)) */
    seq_t tt  = (seq_t)t;
    sum += tt * (tt + 1) / 2;              /* sum 1..t */

    return sum;
}
