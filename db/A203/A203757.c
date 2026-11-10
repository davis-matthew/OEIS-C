/*
 * A203757  Square root of v(2n)/v(2n-1), where v=A203755.
 * https://oeis.org/A203757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 * Product_{k=1..n-1} (k^4 + n^4) [since v(2n)/v(2n-1)
 *            = Product_{i=1..2n-1} (floor(i/2)^4 + n^4)]
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N4 = N2 * N2;
    seq_t result = N2;
    for (idx_t k = 1; k < n; ++k) {
        seq_t K = (seq_t)k;
        seq_t K2 = K * K;
        result *= (K2 * K2 + N4);
    }
    return result;
}
