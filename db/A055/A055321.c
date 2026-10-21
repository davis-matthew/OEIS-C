/*
 * A055321  Number of labeled trees with n nodes and 9 leaves.
 * https://oeis.org/A055321
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n-9)^2 * (n-8)^2 *
 *            (n-7)*(n-6)*(n-5)*(n-4)*(n-3)*(n-2) * (9*n^4 - 270*n^3 +
 *            2967*n^2 - 14098*n + 24352) / 2106910310400 valid for n ≥ 10.
 *            The division is exact
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 13, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Helpers:   mul_cancel
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_cancel.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 10.  For smaller n
       the function returns 0, but those values are not part of
       the OEIS entry. */
    if (n < 10) return 0;

    /* Denominator prime factor counts of 2106910310400:
       2^18 * 3^8 * 5^2 * 7^2. */
    int denom[4] = {18, 8, 2, 2};

    seq_t prod = 1;

    /* n! (full factorial).  The trailing 9! factor will be cancelled
       by the denominator array. */
    for (idx_t i = 2; i <= n; ++i)
        mul_cancel(&prod, (seq_t)i, denom);

    /* Multiply by (n-9)^2 * (n-8)^2 * (n-7)*(n-6)*(n-5)*(n-4)*(n-3)*(n-2) */
    idx_t m9 = n - 9;
    mul_cancel(&prod, (seq_t)m9, denom);
    mul_cancel(&prod, (seq_t)m9, denom);             /* (n-9)^2 */

    idx_t m8 = n - 8;
    mul_cancel(&prod, (seq_t)m8, denom);
    mul_cancel(&prod, (seq_t)m8, denom);             /* (n-8)^2 */

    mul_cancel(&prod, (seq_t)(n - 7), denom);
    mul_cancel(&prod, (seq_t)(n - 6), denom);
    mul_cancel(&prod, (seq_t)(n - 5), denom);
    mul_cancel(&prod, (seq_t)(n - 4), denom);
    mul_cancel(&prod, (seq_t)(n - 3), denom);
    mul_cancel(&prod, (seq_t)(n - 2), denom);

    /* Polynomial part */
    seq_t ns  = (seq_t)n;
    seq_t ns2 = ns * ns;          /* n^2 */
    seq_t ns3 = ns2 * ns;         /* n^3 */
    seq_t ns4 = ns3 * ns;         /* n^4 */

    seq_t poly = (seq_t)9   * ns4
               - (seq_t)270 * ns3
               + (seq_t)2967 * ns2
               - (seq_t)14098 * ns
               + (seq_t)24352;

    mul_cancel(&prod, poly, denom);

    /* If any denominator factors remain (should be none), divide
       them out.  This guards against accidental cancellation
       omissions and keeps the result exact. */
    seq_t denom_val = 1;
    static const uint64_t primes[4] = {2ULL, 3ULL, 5ULL, 7ULL};
    for (int pi = 0; pi < 4; ++pi) {
        for (int c = 0; c < denom[pi]; ++c)
            denom_val *= (seq_t)primes[pi];
    }
    if (denom_val != 1)
        prod /= denom_val;

    return prod;
}
