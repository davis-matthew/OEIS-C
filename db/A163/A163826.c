/*
 * A163826  G.f.: Sum_{n>=1} n * 2^(n^2) * x^n / (1 - 2^n*x)^(n+1).
 * https://oeis.org/A163826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * 2^n * (2^n + 1)^(n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 55 (2026-11-01)
 * Helpers:   ipow_i32, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* 2^n as a seq_t */
    seq_t twon = pow2(n);

    /* (2^n + 1) as the base for the exponent */
    seq_t base = twon + 1;

    /* (2^n + 1)^(n-1) */
    seq_t power = ipow_i32(base, (int)(n - 1));

    /* 2^n * (2^n + 1)^(n-1) */
    seq_t temp = twon * power;

    /* multiply by n */
    seq_t result = ((seq_t)n) * temp;

    return result;
}
