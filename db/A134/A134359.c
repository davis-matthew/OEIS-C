/*
 * A134359  Coefficient of y^(n-4) in expansion of (y+n!)^n.
 * https://oeis.org/A134359
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n!)^4 * n*(n-1)*(n-2)*(n-3) / 24
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0; /* offset: sequence starts at n = 4 */

    /* compute n! */
    seq_t fact = factorial(n);

    /* (n!)^4  -- multiply four times to avoid a helper function */
    seq_t fact4 = fact * fact * fact * fact;

    /* product n*(n-1)*(n-2)*(n-3)   and divide by 24 first */
    seq_t prod       = (seq_t)n * (n-1) * (n-2) * (n-3);
    seq_t prod_div24 = prod / (seq_t)24;

    /* final result */
    return fact4 * prod_div24;
}
