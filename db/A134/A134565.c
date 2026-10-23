/*
 * A134565  Expansion of reversion of (x - 2*x^2) / (1 - x)^3.
 * https://oeis.org/A134565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -(-1)^n * binomial(n + ⌊n/2⌋, n - ⌊n/2⌋) / (2*⌊n/2⌋ + 1)
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 22, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (uses the identity a(2k) = -binom(3k,k)/(2k+1), a(2k+1) =  binom(3k+1,k+1)/(2k+1) */

/* Include the binomial utility */
#include "../utils/binomial.h"

seq_t
a(idx_t n)
{
    /* n >= 1 is assumed by the caller */
    idx_t k = n / 2;                /* floor(n/2) */

    seq_t denom = (seq_t)(2 * k + 1);

    if (n % 2 == 0) {               /* even n = 2k */
        seq_t res = binomial(3 * k, k);
        res /= denom;               /* exact division */
        return -res;
    } else {                        /* odd n = 2k+1 */
        seq_t res = binomial(3 * k + 1, k + 1);
        res /= denom;               /* exact division */
        return res;
    }
}
