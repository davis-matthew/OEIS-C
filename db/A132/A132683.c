/*
 * A132683  a(n) = binomial(2^n + n, n).
 * https://oeis.org/A132683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2^n + n, n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* binom(1,0) = 1 */

    seq_t two_pow = pow2(n);              /* 2^n      */
    seq_t result  = (seq_t)1;             /* running product */

    for (idx_t i = 1; i <= n; ++i) {
        /* numerator = 2^n + i  (no need to add n)   */
        seq_t num = two_pow + (seq_t)i;
        seq_t den = (seq_t)i;

        /* Reduce (num,den) by their GCD */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* Reduce (result,den) by their GCD */
        g = gcd(result, den);
        result /= g;
        den /= g;                   /* den should now be 1 */

        /* Multiply into result */
        result *= num;
    }

    return result;
}
