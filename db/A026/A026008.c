/*
 * A026008  a(n) = T(n, floor(n/2)), where T = Catalan triangle (A008315).
 * https://oeis.org/A026008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(n, floor(n/2)) where T is Catalan triangle. Closed
 *            forms: n = 2k => a(n) = binomial(2k+2, k+1) / (k+2) n = 2k+1 =>
 *            a(n) = 3 * binomial(2k+2, k) / (k+3)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if ((n & 1U) == 0U) {
        idx_t k  = n >> 1;
        seq_t v  = binomial(2 * k + 2, k + 1);
        return v / (seq_t)(k + 2);
    } else {
        idx_t k  = (n - 1U) >> 1;
        seq_t v  = binomial(2 * k + 2, k);
        return (v * 3) / (seq_t)(k + 3);
    }
}
