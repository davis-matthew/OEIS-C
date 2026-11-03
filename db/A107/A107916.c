/*
 * A107916  a(n) = binomial(n+3,2)*binomial(n+4,3)*binomial(n+5,5)/12.
 * https://oeis.org/A107916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+3,2)*binomial(n+4,3)*binomial(n+5,5)/12
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 21, u64 n <= 221, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 923 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   Computes A107916: a(n) = (n+1)(n+2)^3(n+3)^3(n+4)^2(n+5) / 17280.
   The product is reduced during multiplication to keep intermediate
   values small for all supported seq_t types.
*/
#include <stddef.h>

static inline void mul_reduce(seq_t *res, seq_t f, unsigned int *denom)
{
    while ((*denom % 2 == 0) && (f % 2 == 0)) {
        f /= 2;
        *denom /= 2;
    }
    while ((*denom % 3 == 0) && (f % 3 == 0)) {
        f /= 3;
        *denom /= 3;
    }
    while ((*denom % 5 == 0) && (f % 5 == 0)) {
        f /= 5;
        *denom /= 5;
    }
    *res *= f;
}

seq_t a(idx_t n)
{
    unsigned int denom = 17280;        /* 2^7 * 3^3 * 5 */
    seq_t result = 1;

    /* (n+1) */
    mul_reduce(&result, (seq_t)n + 1, &denom);

    /* (n+2)^3  (three copies) */
    for (int i = 0; i < 3; ++i)
        mul_reduce(&result, (seq_t)n + 2, &denom);

    /* (n+3)^3  (three copies) */
    for (int i = 0; i < 3; ++i)
        mul_reduce(&result, (seq_t)n + 3, &denom);

    /* (n+4)^2  (two copies) */
    for (int i = 0; i < 2; ++i)
        mul_reduce(&result, (seq_t)n + 4, &denom);

    /* (n+5) */
    mul_reduce(&result, (seq_t)n + 5, &denom);

    /* denom should now be 1; if not, divide remaining (should not happen) */
    if (denom != 1) {
        result /= denom;
    }

    return result;
}
