/*
 * A090138  Denominator of the probability that the sum of n uniform picks on
 *          [0,1] is first greater than 2 (i.e., the sum of n-1 is not).
 * https://oeis.org/A090138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 35,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A090138
 * a(n) = denominator((n-2)*(2^(n-1)-n)/n!)
 *        i.e. the reduced denominator of (n-2)*(2^(n-1)-n) / n!
 *
 * Compute: for n >= 3, let N = (n-2)*(2^(n-1)-n).
 * Denominator = n! / gcd(N, n!).
 * To avoid overflow we cancel common factors
 * while building the factorial.
 */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"   /* pow2(x) returns 2^x as seq_t */
#include "../utils/gcd.h"    /* gcd(a,b) returns greatest common divisor */

seq_t a(idx_t n)
{
    /* For n = 1,2 the probability is 0/1, so the denominator is 1. */
    if (n <= 2) {
        return (seq_t)1;
    }

    /* Convert n to seq_t for computations. */
    seq_t sn = (seq_t)n;

    /* Compute numerator: N = (n-2)*(2^(n-1)-n). */
    seq_t pow2_n_1 = pow2(n - 1);          /* 2^(n-1) as seq_t */
    seq_t sub = pow2_n_1 - sn;             /* 2^(n-1) - n   */
    seq_t N = ((seq_t)(n - 2)) * sub;       /* (n-2)*(2^(n-1)-n) */

    /* Prepare for factorial with cancellation. */
    seq_t denom = (seq_t)1;      /* running product of remaining factorial factors */
    seq_t tempN = N;             /* numerator partially cancelled */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t g  = gcd(ii, tempN);      /* common factor between i and numerator */
        if (g != (seq_t)0) {
            tempN /= g;                 /* cancel this part from numerator   */
            ii /= g;                    /* keep only coprime part for denom   */
        }
        denom *= ii;                     /* multiply remaining factor into denom */
    }

    /* denom now equals n! / gcd(N, n!) */
    return denom;
}
