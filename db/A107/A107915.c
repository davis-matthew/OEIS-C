/*
 * A107915  a(n) = binomial(n+4,4)*binomial(n+5,4)*binomial(n+6,4)/75.
 * https://oeis.org/A107915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+4,4)*binomial(n+5,4)*binomial(n+6,4)/75
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 16, u64 n <= 124, u128 n <= 1000,
 *            big any n
 * Helpers:   reduce_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reduce_val.h"

seq_t a(idx_t n)
{
    /* We need to cancel the denominator 2^9 * 3^4 * 5^2.
       The product of numerators is
          (n+1) (n+2)^2 (n+3)^3 (n+4)^3 (n+5)^2 (n+6)         */
    int c2 = 9, c3 = 4, c5 = 2;
    seq_t result = 1, val;
    int i;

    /* n+1 (once) */
    val = (seq_t)(n + 1);
    result *= reduce_val(val, &c2, &c3, &c5);

    /* n+2 (twice) */
    val = (seq_t)(n + 2);
    for (i = 0; i < 2; ++i) result *= reduce_val(val, &c2, &c3, &c5);

    /* n+3 (three times) */
    val = (seq_t)(n + 3);
    for (i = 0; i < 3; ++i) result *= reduce_val(val, &c2, &c3, &c5);

    /* n+4 (three times) */
    val = (seq_t)(n + 4);
    for (i = 0; i < 3; ++i) result *= reduce_val(val, &c2, &c3, &c5);

    /* n+5 (twice) */
    val = (seq_t)(n + 5);
    for (i = 0; i < 2; ++i) result *= reduce_val(val, &c2, &c3, &c5);

    /* n+6 (once) */
    val = (seq_t)(n + 6);
    result *= reduce_val(val, &c2, &c3, &c5);

    /* Any remaining denominator factors (should be none) */
    if (c2 > 0 || c3 > 0 || c5 > 0) {
        seq_t denom = 1;
        while (c2-- > 0) denom *= 2;
        while (c3-- > 0) denom *= 3;
        while (c5-- > 0) denom *= 5;
        result /= denom;
    }

    return result;
}
