/*
 * A023418  Denominator of n*(n-3)*(3*n^2 - 6*n + 2)/(3*(n-1)*(n-2)).
 * https://oeis.org/A023418
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Denominator of n*(n-3)*(3*n^2 - 6*n + 2)/(3*(n-1)*(n-2))
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 233, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n < 3 the value is undefined; the function returns 0 in that case. */
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t nseq = (seq_t)n;

    seq_t term1 = nseq - (seq_t)3;
    seq_t term2 = (seq_t)3 * nseq * nseq - (seq_t)6 * nseq + (seq_t)2;

    seq_t num = nseq * term1 * term2;
    seq_t den = (seq_t)3 * (nseq - (seq_t)1) * (nseq - (seq_t)2);

    seq_t g = gcd(num, den);
    return den / g;
}
