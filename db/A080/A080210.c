/*
 * A080210  Lower bound for the multiplicity of a disjoint covering system of
 *          congruences.
 * https://oeis.org/A080210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(Lpf(n)*phi(n)/n) + 1
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   _lpf_calc, _phi_calc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_lpf_calc.h"
#include "../utils/_phi_calc.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* a(1) = 1 by definition */

    seq_t lp   = _lpf_calc(n);    /* Lpf(n) */
    seq_t phi  = _phi_calc(n);    /* φ(n)   */
    __uint128_t numer = (__uint128_t)lp * (__uint128_t)phi;
    seq_t div = (seq_t)(numer / n);
    return div + (seq_t)1;
}
