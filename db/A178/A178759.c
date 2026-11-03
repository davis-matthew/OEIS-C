/*
 * A178759  Expansion of e.g.f. 3*x*exp(x)*(exp(x)-1)^2.
 * https://oeis.org/A178759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n - 3*2^n + 3)*n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 969 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* compute 3^n and 2^n */
    seq_t pow3 = ipow(3, (seq_t)n);
    seq_t pow2 = ipow(2, (seq_t)n);

    /* use (3^n + 3 - 3*2^n) to avoid unsigned underflow */
    seq_t result = pow3;
    result += 3;                 /* add the constant 3 */
    result -= 3 * pow2;          /* subtract 3*2^n */

    /* multiply by n (converted to seq_t) */
    result *= (seq_t)n;

    return result;
}
