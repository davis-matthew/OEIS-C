/*
 * A129995  a(n) = (n + 1)*(n^2 + 2)*(n^3 + 3)*(n^4 + 4)/4!.
 * https://oeis.org/A129995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n^2+2)*(n^3+3)*(n^4+4)/4!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 12, u64 n <= 115, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Helper to cancel a factor with the remaining denominator. */
static void cancel_factor(seq_t *factor, seq_t *denom)
{
    seq_t g = gcd(*factor, *denom);
    if (g > 1)
    {
        *factor /= g;
        *denom  /= g;
    }
}

seq_t a(idx_t n)
{
    /* Prepare the four factors of the product. */
    seq_t f0 = (seq_t)(n + 1);                    /* n + 1                      */
    seq_t n2 = (seq_t)n * (seq_t)n;               /* n^2                        */
    seq_t f1 = n2 + 2;                            /* n^2 + 2                    */
    seq_t n3 = n2 * (seq_t)n;                     /* n^3                        */
    seq_t f2 = n3 + 3;                            /* n^3 + 3                    */
    seq_t n4 = n3 * (seq_t)n;                     /* n^4                        */
    seq_t f3 = n4 + 4;                            /* n^4 + 4                    */

    /* Cancel factors of 4! (i.e., 24) to avoid overflow. */
    seq_t denom = 24;
    cancel_factor(&f0, &denom);
    if (denom > 1) cancel_factor(&f1, &denom);
    if (denom > 1) cancel_factor(&f2, &denom);
    if (denom > 1) cancel_factor(&f3, &denom);

    /* Multiply the remaining factors. */
    seq_t result = 1;
    result *= f0;
    result *= f1;
    result *= f2;
    result *= f3;

    /* Any remaining factor of the denominator (if any) divides the product. */
    if (denom > 1)
        result /= denom;

    return result;
}
