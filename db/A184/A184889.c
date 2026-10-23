/*
 * A184889  a(n) = (5^n/n!^2) * Product_{k=0..n-1} (10k+2)*(10k+3).
 * https://oeis.org/A184889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5^n / n!^2) * ∏_{k=0..n-1} (10k+2)(10k+3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Computes the n-th term of OEIS sequence A184889.
   idx_t and seq_t are defined by the build system. */
seq_t a(idx_t n)
{
    seq_t result = 1;                      /* a(0) = 1 */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Construct numerator 5*(10i-8)*(10i-7) and denominator i*i */
        seq_t num = 5;
        num *= (seq_t)(10*i - 8);
        num *= (seq_t)(10*i - 7);

        seq_t denom = (seq_t)i * (seq_t)i;

        /* Cancel common factors between numerator and denominator */
        seq_t g = gcd(num, denom);
        if (g > 1) {
            num   /= g;
            denom /= g;
        }

        /* Further cancel with the current result to keep numbers small */
        g = gcd(result, denom);
        if (g > 1) {
            result /= g;
            denom   /= g;
        }

        /* After the cancellations, denom should be 1.
           Multiply the remaining numerator into the result. */
        result *= num;
        /* The sequence definition guarantees integer values,
           so denom must be 1 at this point. */
    }
    return result;
}
