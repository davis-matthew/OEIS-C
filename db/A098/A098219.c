/*
 * A098219  a(n) = floor(sigma(sigma(n))/n) = floor(A051027(n)/n).
 * https://oeis.org/A098219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sigma(sigma(n)) / n)
 * Range:     u8 n <= 53, u16 n <= 9658, u32 n <= 9658, u64 n <= 9658,
 *            u128 n <= 9658, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* n >= 1 is assumed */
    seq_t sigma1 = sigma_of((idx_t)n);
    seq_t sigma2 = sigma_of((idx_t)sigma1);
    return sigma2 / (seq_t)n;
}
