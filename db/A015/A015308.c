/*
 * A015308  Gaussian binomial coefficient [ n,5 ] for q = -4.
 * https://oeis.org/A015308
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ∏_{i=1..5} ((-4)^(n-i+1)-1)/((-4)^i-1) (q = -4)
 * Range:     i8 n <= 5, i16 n <= 5, i32 n <= 7, i64 n <= 11, i128 n <= 17,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/big_pow.h"

/* Compute the Gaussian binomial coefficient [n,5] for q = -4.
 * The result is an integer for all n ≥ 5.  Values are
 * computed with exact integer arithmetic, reducing after each
 * multiplication to keep intermediate results small.
 */
seq_t a(idx_t n)
{
    if (n < 5) return 0;                 /* undefined below offset */

    seq_t num = 1;    /* numerator of the accumulated product   */
    seq_t den = 1;    /* denominator of the accumulated product */
    int   sign = 1;   /* overall sign of the result             */

    for (idx_t i = 1; i <= 5; ++i) {
        /* factors:  ((-4)^(n-i+1)-1)  /  ((-4)^i-1) */
        seq_t exp1  = (seq_t)(n - i + 1);
        seq_t pow1  = big_pow((seq_t)-4, exp1);
        seq_t num_f = pow1 - 1;

        seq_t exp2  = (seq_t)i;
        seq_t pow2  = big_pow((seq_t)-4, exp2);
        seq_t den_f = pow2 - 1;

        /* record sign and use absolute values for gcd */
        if (num_f < 0) { sign = -sign; num_f = -num_f; }
        if (den_f < 0) { sign = -sign; den_f = -den_f; }

        /* reduce the factor itself */
        seq_t g = gcd(num_f, den_f);
        num_f /= g;
        den_f /= g;

        /* reduce cross terms with the running product */
        seq_t g2 = gcd(num, den_f);
        num /= g2;
        den_f /= g2;

        seq_t g3 = gcd(den, num_f);
        den /= g3;
        num_f /= g3;

        /* multiply the reduced factors into the running product */
        num = num * num_f;
        den = den * den_f;

        /* final reduction to keep numbers small */
        seq_t g4 = gcd(num, den);
        num /= g4;
        den /= g4;
    }

    /* The result is an integer; apply the accumulated sign. */
    seq_t res = num / den;   /* den divides num exactly */
    return (sign < 0) ? -res : res;
}
