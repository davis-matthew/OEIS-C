/*
 * A061433  Largest n-digit square.
 * https://oeis.org/A061433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (ceiling(10^(n/2)) - 1)^2, equivalent to the largest
 *            n‑digit perfect square
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"

/* Returns the largest square with exactly n decimal digits.
   n >= 1. */
seq_t a(idx_t n)
{
    /* 10^n as a seq_t. */
    seq_t pow10n = pow10(n);

    /* 10^n - 1.  The integer type conversion of 1 to seq_t
       works for all allowed seq_t definitions. */
    seq_t max_n_digit = pow10n - (seq_t)1;

    /* Floor of the square root of that number. */
    seq_t root = floor_sqrt(max_n_digit);

    /* The desired square. */
    return root * root;
}
