/*
 * A024578  a(n) = [ 1/{n/e} ], {x} := x - [ x ].
 * https://oeis.org/A024578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(1/{n/e}) where {x} = x - floor(x)
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Rational approximation of e:   e ≈ 2718281828459045 / 1000000000000000  */
static const uint64_t E_NUM = 2718281828459045ULL;
static const uint64_t E_DEN = 1000000000000000ULL;

seq_t a(idx_t n)
{
    /* use 128‑bit arithmetic for the intermediate product */
    __int128 mult = (__int128)n * (__int128)E_DEN;        /* n * E_DEN  */

    uint64_t k   = (uint64_t)(mult / E_NUM);             /* floor(n*E_DEN/E_NUM) */
    uint64_t rem = (uint64_t)(mult % E_NUM);             /* fractional part numerator */

    /* rem > 0 for all n >= 1 because e is irrational and the approximation is exact to 15 digits */
    uint64_t val = E_NUM / rem;                          /* floor(1/{n/e})  */

    return (seq_t)val;
}
