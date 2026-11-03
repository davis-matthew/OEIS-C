/*
 * A135424  a(n) = n!^number of decimal digits of n!.
 * https://oeis.org/A135424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n!)^(digit_count(n!))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-01)
 * Helpers:   ipow, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"
#include "../utils/num_digits.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    seq_t f = factorial(n);
    int d = num_digits(f);
    seq_t exp = (seq_t)d;
    return ipow(f, exp);
}
