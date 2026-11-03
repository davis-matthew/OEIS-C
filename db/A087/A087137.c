/*
 * A087137  a(n) is the number of permutations in the symmetric group S_n that
 *          contain an odd cycle.
 * https://oeis.org/A087137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! if n is odd a(n) = n! – ((n-1)!!)^2 if n is even
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 435 (2026-11-01)
 * Helpers:   double_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/double_factorial.h"

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
    if (n == 0)
        return 0;

    seq_t fact = factorial(n);

    if (n & 1)    /* odd n */
        return fact;

    seq_t df  = double_factorial(n - 1); /* (n-1)!! */
    seq_t sq  = df * df;                 /* ((n-1)!!)^2 */
    return fact - sq;
}
