/*
 * A184890  a(n) = C(2n,n) * (5^n/n!^2) * Product_{k=0..n-1} (5k+2)*(5k+3).
 * https://oeis.org/A184890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(2n,n) * (5^n / n!^2) * ∏_{k=0..n-1} (5k+2)(5k+3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    seq_t result = (seq_t)1;

    for (idx_t i = 0; i < n; ++i)
    {
        /* denominator = (i+1)^4 */
        seq_t step  = (seq_t)(i + 1);
        seq_t denom = step;
        denom *= step;
        denom *= step;
        denom *= step;

        /* numerator factors */
        seq_t f1 = (seq_t)(2 * i + 2);
        seq_t f2 = (seq_t)(2 * i + 1);
        seq_t f3 = (seq_t)5;
        seq_t f4 = (seq_t)(5 * i + 2);
        seq_t f5 = (seq_t)(5 * i + 3);

        /* helper to cancel common factors between a factor and the denominator,
         * then multiply the result by that reduced factor */
        #define MUL_REDUCE(f)                    \
            do {                                 \
                seq_t g = seq_gcd(f, denom);      \
                if (g > (seq_t)1) {               \
                    f  /= g;                      \
                    denom /= g;                   \
                }                                  \
                g = seq_gcd(result, denom);      \
                if (g > (seq_t)1) {               \
                    result /= g;                  \
                    denom  /= g;                  \
                }                                  \
                result *= f;                      \
            } while (0)

        MUL_REDUCE(f1);
        MUL_REDUCE(f2);
        MUL_REDUCE(f3);
        MUL_REDUCE(f4);
        MUL_REDUCE(f5);

        /* any remaining denominator must now be 1, but just in case */
        if (denom != (seq_t)1) {
            result /= denom;
            denom = (seq_t)1;
        }

        #undef MUL_REDUCE
    }

    return result;
}
