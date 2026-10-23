/*
 * A017808  Binomial coefficients C(92,n).
 * https://oeis.org/A017808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(92,n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 18, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n > 92) return 0;
    idx_t k = n;
    if (k > 92 - k) k = 92 - k;   /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        idx_t numer = 92 - k + i;
        idx_t denom = i;
        unsigned __int128 temp = (unsigned __int128)result * numer;
        temp /= denom;          /* division is exact */
        result = (seq_t)temp;
    }
    return result;
}
