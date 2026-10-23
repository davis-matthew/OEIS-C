/*
 * A159645  Numerator of Hermite(n, 7/19).
 * https://oeis.org/A159645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 19^n * Hermite(n, 7/19) a(n) = Σ_{k=0}^{⌊n/2⌋} (-1)^k *
 *            n!/(k!(n-2k)!) * 361^k * 14^{n-2k} where 361 = 19^2 and 14 is
 *            7*2. The implementation uses only integer arithmetic on the
 *            supplied seq_t type
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 10, i128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"  /* for big_pow(base, exp) */

seq_t a(idx_t n)
{
    /* iterator bounds: 0 .. floor(n/2) */
    idx_t maxk = n / 2;           /* uint64_t division is fine */
    seq_t sum = 0;                /* running total */

    seq_t factnk = 1;             /* n!/(k! (n-2k)!) initially 1 for k=0 */
    seq_t pow361 = 1;             /* 361^k, initially 1 */
    seq_t pow14  = big_pow((seq_t)14, (seq_t)n);  /* 14^n */
    int sign = 1;                 /* (-1)^k, flips each iteration */

    for (idx_t k = 0; k <= maxk; ++k) {
        /* term = sign * factnk * pow361 * pow14 */
        seq_t term = factnk * pow361 * pow14;
        if (sign < 0)
            term = -term;
        sum += term;

        /* prepare for next k */
        if (k == maxk) break;   /* last term reached */

        /* Update factorial part: factnk+1 = factnk * (n-2k)(n-2k-1)/(k+1) */
        seq_t num1 = (seq_t)(n - 2*k);          /* n-2k */
        seq_t num2 = (seq_t)(n - 2*k - 1);      /* n-2k-1 */
        seq_t denom = (seq_t)(k + 1);           /* k+1 */
        factnk = factnk * num1 * num2 / denom;

        /* Update powers */
        pow361 = pow361 * (seq_t)361;    /* multiply by 19^2 */
        pow14  = pow14 / (seq_t)196;      /* divide by 14^2 = 196 */

        /* Flip sign */
        sign = -sign;
    }

    return sum;
}
