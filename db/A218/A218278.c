/*
 * A218278  Convolution of level 4 of the divisor function.
 * https://oeis.org/A218278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (16*(sigma_3(n)+2*sigma(n)-3*n*sigma(n)) + *
 *            (1+(-1)^n)*(173*sigma_3(n)-21*sigma_3(2*n)+28*sigma(n)-12*sigma(2*n)-168*n*sigma(n)+72*n*sigma(2*n)))
 *            / 768
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   _sigma, _sigma_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_sigma.h"
#include "../utils/_sigma_pow.h"

seq_t a(idx_t n)
{
    /* Compute required divisor sums. */
    seq_t sigma_n   = _sigma(n);
    seq_t sigma3_n  = _sigma_pow(n, 3);
    idx_t two_n     = n * 2;
    seq_t sigma_2n  = _sigma(two_n);
    seq_t sigma3_2n = _sigma_pow(two_n, 3);

    /* First bracket: T1 = sigma_3(n)+2*sigma(n)-3*n*sigma(n) */
    seq_t T1 = sigma3_n + 2 * sigma_n - 3 * (seq_t)n * sigma_n;

    /* Second bracket: factor = 1+(-1)^n (equals 2 if n even, 0 if n odd) */
    unsigned factor = (n & 1) ? 0 : 2;
    seq_t inner = 0;
    if (factor) {
        /* Compute the complex expression only when factor ≠ 0. */
        inner = 173 * sigma3_n
              - 21 * sigma3_2n
              + 28 * sigma_n
              - 12 * sigma_2n
              - 168 * (seq_t)n * sigma_n
              + 72 * (seq_t)n * sigma_2n;
    }
    seq_t T2 = factor * inner;

    /* Numerator with common denominator 768 (since 768 = 16*48). */
    seq_t numerator = 16 * T1 + T2;

    /* Final value.  Division is exact. */
    return numerator / 768;
}
