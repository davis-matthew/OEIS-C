/*
 * A165642  Number of ways to assemble an n-cube from 2n labeled (n-1)-cubes
 *          with labeled vertices, where left-handed and right-handed
 *          counterparts are considered distinct.
 * https://oeis.org/A165642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * ((2n-2)!!)^(2n-1) * (2n-1)! (Offset: a(1) = 2)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow.h"

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
        return 0; /* out of bounds, unused */

    /* 2^{n-1} */
    seq_t two_pow = ipow((seq_t)2, (seq_t)(n - 1));

    /* (n-1)! */
    seq_t fact_n1 = factorial(n - 1);

    /* double factorial (2n-2)!! = 2^{n-1} * (n-1)! */
    seq_t doublefact = two_pow * fact_n1;

    /* exponent 2n-1 */
    idx_t exp = 2 * n - 1;

    /* ((2n-2)!!)^(2n-1) */
    seq_t pow_doublefact = ipow(doublefact, (seq_t)exp);

    /* (2n-1)! */
    seq_t fact_2n1 = factorial(exp);

    /* final result */
    return (seq_t)2 * pow_doublefact * fact_2n1;
}
