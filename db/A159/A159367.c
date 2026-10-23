/*
 * A159367  Number of n X n arrays of squares of integers summing to 7.
 * https://oeis.org/A159367
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            n^2*(n-1)*(n+1)*(n^2-2)*(n^2-3)*(n^6-15*n^4+74*n^2+720)/5040
 * Range:     u8 none, u16 n <= 4, u32 n <= 9, u64 n <= 43, u128 n <= 100,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0; /* defined only for n >= 2 */

    seq_t n2 = (seq_t)n * (seq_t)n;               /* n^2 */
    seq_t n4 = n2 * n2;                          /* n^4 */
    seq_t n6 = n2 * n4;                          /* n^6 */

    /* Compute the last factor without unsigned underflow */
    seq_t term = n6 + (seq_t)74 * n2 + (seq_t)720;
    term -= (seq_t)15 * n4;                     /* n^6 - 15*n^4 + 74*n^2 + 720 */

    /* Numerator factors */
    seq_t factors[6] = {
        n2,                      /* n^2 */
        (seq_t)(n - 1),          /* n-1 */
        (seq_t)(n + 1),          /* n+1 */
        n2 - (seq_t)2,           /* n^2-2 */
        n2 - (seq_t)3,           /* n^2-3 */
        term                     /* the remaining factor */
    };

    seq_t res = (seq_t)1;
    seq_t denom = (seq_t)5040; /* 2^4 * 3^2 * 5 * 7 */

    for (int i = 0; i < 6; ++i) {
        seq_t g = gcd(factors[i], denom);
        if (g != (seq_t)1) {
            factors[i] /= g;
            denom /= g;
        }
        res *= factors[i];
    }

    /* After processing all factors, denom should be 1 */
    return res;
}
