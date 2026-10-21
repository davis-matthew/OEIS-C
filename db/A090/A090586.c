/*
 * A090586  Denominator of Sum/Product of first n numbers.
 * https://oeis.org/A090586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (n!)/(gcd(n!, n(n+1)/2)), the denominator of
 *            (1+2+...+n)/(1·2·...·n)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)1;
    /* remaining part of the sum to cancel */
    seq_t rem = (seq_t)n * ((seq_t)n + 1) / 2;
    seq_t denom = 1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t d = gcd(ii, rem);   /* common factor between i and remaining sum part */
        rem /= d;
        denom *= ii / d;
    }
    return denom;
}
