/*
 * A105524  Diagonal sums of number triangle A105522.
 * https://oeis.org/A105524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   2*(n+2)*(13*n^2-35*n+6)*a(n) + 24*(n-8)*a(n-1) +
 *            (91*n^3-375*n^2+152*n+372)*a(n-2) + 12*(8-n)*a(n-3) -
 *            4*(n-3)*(13*n^2-9*n-16)*a(n-4) = 0, a(0)=1, a(1)=-2, a(2)=2,
 *            a(3)=0
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 19, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static cache for already computed values. */
    static seq_t *dp = NULL;     /* array of computed terms       */
    static idx_t dp_size = 0;    /* number of elements currently in dp */

    /* Initialise with the first four terms, which form the base of the recurrence. */
    if (dp_size == 0) {
        dp_size = 4;                           /* indices 0..3 will be stored   */
        dp = seq_malloc(dp_size);
        dp[0] = (seq_t)1;
        dp[1] = (seq_t)-2;
        dp[2] = (seq_t)2;
        dp[3] = (seq_t)0;
    }

    /* If the requested term is already cached, return it. */
    if (n < dp_size)
        return dp[n];

    /* Grow the cache to accommodate the new term(s). */
    idx_t newsize = n + 1;
    dp = seq_realloc(dp, newsize);

    /* Compute all missing terms up to n (inclusive). */
    for (idx_t i = dp_size; i <= n; ++i) {
        seq_t ii = (seq_t)i;                 /* use ii to avoid repeated casts */

        /* Terms of the recurrence. */
        seq_t coeff1 = (seq_t)24 * (ii - (seq_t)8);
        seq_t term1   = coeff1 * dp[i - 1];

        seq_t coeff2 = (seq_t)91 * ii * ii * ii
                     - (seq_t)375 * ii * ii
                     + (seq_t)152 * ii
                     + (seq_t)372;
        seq_t term2   = coeff2 * dp[i - 2];

        seq_t coeff3 = (seq_t)12 * ((seq_t)8 - ii);
        seq_t term3   = coeff3 * dp[i - 3];

        seq_t coeff4 = (seq_t)13 * ii * ii
                     - (seq_t)9 * ii
                     - (seq_t)16;
        seq_t term4   = -( (seq_t)4 * (ii - (seq_t)3) * coeff4 ) * dp[i - 4];

        seq_t numerator = term1 + term2 + term3 + term4;

        seq_t denom = (seq_t)2 * (ii + (seq_t)2)
                    * ((seq_t)13 * ii * ii - (seq_t)35 * ii + (seq_t)6);

        /* From the recurrence: a(i) = -numerator / denom.  Division is exact. */
        dp[i] = -numerator / denom;
    }

    dp_size = newsize;
    return dp[n];
}
