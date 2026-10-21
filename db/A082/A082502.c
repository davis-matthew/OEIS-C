/*
 * A082502  Erroneous version of A052200.
 * https://oeis.org/A082502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (4*(n+1))^(2*n) for all n ≥ 0 (Note: the OEIS entry
 *            A082502 has an erroneous value for n = 3; the sequence contains
 *            1677216 instead of the correctly computed 16,777,216. The
 *            following implementation reproduces that erroneous entry.)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 3, i64 n <= 5, i128 n <= 5,
 *            big any n
 * Helpers:   pow_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_big.h"

seq_t a(idx_t n)
{
    /* Handle the erroneous entry for n = 3 */
    if (n == 3)
        return (seq_t)1677216;

    seq_t base = (seq_t)(4 * (n + 1));
    idx_t exp  = 2 * n;
    return pow_big(base, exp);
}
