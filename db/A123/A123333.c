/*
 * A123333  a(n) = 3^n*(Gamma(n+1/3)/Gamma(1/3) + (n-1)!).
 * https://oeis.org/A123333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \prod_{k=0}^{n-1} (3k+1) + 3^n * (n-1)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   factorial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* For n < 1 the offset is not defined; return 0 for safety. */
    if (n == 0) {
        return 0;
    }

    /* Compute the product \prod_{k=0}^{n-1} (3k+1). */
    seq_t prod = 1;
    for (idx_t k = 0; k < n; ++k) {
        prod *= (seq_t)(3 * k + 1);
    }

    /* Compute the term 3^n * (n-1)! using the helper functions. */
    seq_t term2 = pow_uint(3, n);
    term2 *= factorial(n - 1);

    /* Sum the two parts to obtain a(n). */
    return prod + term2;
}
