/*
 * A077512  Largest n-digit square beginning with n.
 * https://oeis.org/A077512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (floor_sqrt((n+1)*10^(n-d) - 1))^2, where d = digits(n) *
 *            and the resulting square is guaranteed to be in the range
 *            [n*10^(n-d), * (n+1)*10^(n-d)-1]. The leading decimal digits of
 *            a(n) form the * string representation of n, and a(n) has exactly
 *            n digits
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   digits, floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0.
     * No other special cases are needed. */
    if (n == 0)
        return 0;

    /* Number of decimal digits in n. */
    int d = digits(n);                /* 1 <= d <= ~5 for n <= 10000 */
    idx_t exp = n - (idx_t)d;         /* exponent for the power of 10 */

    /* 10^(n-d) as a seq_t; will overflow naturally for very large
     * results, but that is fine for the builds that only need values
     * that fit into seq_t. */
    seq_t pow10_exp = pow10(exp);

    /* Lower and upper bounds for the n-digit numbers starting with n. */
    seq_t L = (seq_t)n * pow10_exp;                       /* n * 10^(n-d) */
    seq_t U = ((seq_t)n + (seq_t)1) * pow10_exp;          /* (n+1)*10^(n-d) */
    U -= (seq_t)1;                                        /* minus 1 */

    /* Largest square <= U.  floor_sqrt returns floor(sqrt(U)). */
    seq_t t = floor_sqrt(U);

    seq_t sq = t * t;
    /* In rare cases the floor square might be smaller than L
     * (e.g., if the range [L,U] contains no square).  Then
     * decrement t until the square lies in the range. */
    while (sq < L) {
        t -= (seq_t)1;
        sq = t * t;
    }

    return sq;
}
