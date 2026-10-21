/*
 * A094760  Duplicate of A083956.
 * https://oeis.org/A094760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n*(n+1)/2 * (10^n - 1) / 9 (OEIS A094760)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 9, i128 n <= 9,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; value for n=0 is arbitrary */
        return 0;

    /* triangular number T(n) = n*(n+1)/2 */
    seq_t tn = (seq_t)n * ((seq_t)n + 1) / 2;

    /* repunit R(n) = (10^n - 1) / 9 */
    seq_t p10 = pow10(n);
    seq_t rep = (p10 - 1) / 9;

    return tn * rep;
}
