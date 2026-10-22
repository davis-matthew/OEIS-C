/*
 * A004070  Table of Whitney numbers W(n,k) read by antidiagonals, where
 *          W(n,k) is maximal number of pieces into which n-space is sliced by
 *          k hyperplanes, n >= 0, k >= 0.
 * https://oeis.org/A004070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy, nice
 * Formula:   W(n,k) = Sum_{i=0..n} binomial(k,i), read by antidiagonals d =
 *            n+k, with k = position within the antidiagonal and n = d - k
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   a004070_isqrt, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/a004070_isqrt.h"

seq_t a(idx_t m) {
    idx_t d = (a004070_isqrt(8 * m + 1) - 1) / 2;
    while (d * (d + 1) / 2 > m) d--;
    while ((d + 1) * (d + 2) / 2 <= m) d++;
    idx_t k = m - d * (d + 1) / 2;
    idx_t n = d - k;
    idx_t top = n < k ? n : k;

    seq_t result = 0;
    for (idx_t i = 0; i <= top; i++) {
        result += binom(k, i);
    }
    return result;
}
