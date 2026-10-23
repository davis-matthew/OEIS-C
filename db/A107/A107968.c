/*
 * A107968  a(n) = (n+1)*(n+2)^3*(n+3)^2*(n+4)*(3n+5)/1440.
 * https://oeis.org/A107968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)^3*(n+3)^2*(n+4)*(3n+5)/1440
 * Range:     u8 none, u16 n <= 6, u32 n <= 32, u64 n <= 551, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd_any
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_any.h"

static inline void reduce_mul(seq_t *result, seq_t *denom, seq_t factor)
{
    seq_t g = gcd_any(factor, *denom);
    factor /= g;
    *denom /= g;
    *result *= factor;
    g = gcd_any(*result, *denom);
    *result /= g;
    *denom /= g;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;

    seq_t f1 = x + 1;            /* n+1 */
    seq_t f2 = x + 2;            /* n+2 (will be used three times) */
    seq_t f5 = x + 3;            /* n+3 (will be used two times) */
    seq_t f7 = x + 4;            /* n+4 */
    seq_t f8 = (seq_t)3 * x + 5; /* 3n+5 */

    seq_t denom = 1440;
    seq_t result = 1;

    reduce_mul(&result, &denom, f1);
    reduce_mul(&result, &denom, f2);
    reduce_mul(&result, &denom, f2);
    reduce_mul(&result, &denom, f2);
    reduce_mul(&result, &denom, f5);
    reduce_mul(&result, &denom, f5);
    reduce_mul(&result, &denom, f7);
    reduce_mul(&result, &denom, f8);

    /* Any remaining denominator factors must divide result now */
    if (denom > 1)
        result /= denom;

    return result;
}
