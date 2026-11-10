/*
 * A089266  Rational knots of determinant 2n+1, counting chiral pairs twice.
 * https://oeis.org/A089266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (phi(2n+1) + 2^omega(2n+1))/2
 * Range:     u8 n <= 145, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 962 (2026-11-07)
 * Helpers:   omega, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/totient.h"
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    idx_t m = (idx_t)(2 * n + 1);
    seq_t phi = totient(m);
    int w = omega((uint64_t)m);
    seq_t u = 1;
    for (int i = 0; i < w; ++i)
        u *= 2;
    return (phi + u) / 2;
}
