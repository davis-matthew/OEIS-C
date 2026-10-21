/*
 * A066558  a(n) = A066557(n)/n.
 * https://oeis.org/A066558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor((10^n - 1)/n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Handle arithmetic for 8/16/32/64-bit seq_t types explicitly
     * to avoid overflow when computing 10^n. For larger seq_t
     * (e.g. arbitrary-precision) we can use the provided pow10
     * utility directly. */
    if (sizeof(seq_t) <= 8) {
        /* n is assumed reasonably small (n <= 20 for values that fit
         * in 64-bit).  Use 128-bit intermediate to fit 10^n precisely. */
        __int128 pow = 1;
        for (idx_t i = 0; i < n; ++i) {
            pow *= 10;
        }
        /* Compute floor((10^n - 1)/n) using the 128-bit integer. */
        __int128 numerator = pow - 1;
        __int128 denom = (__int128)n;
        seq_t result = (seq_t)(numerator / denom);
        return result;
    } else {
        /* seq_t is big enough (e.g. arbitrary precision) */
        seq_t p = pow10(n);        /* 10^n */
        seq_t res = p - 1;         /* 10^n - 1 */
        return res / (seq_t)n;     /* integer division is truncating */
    }
}
