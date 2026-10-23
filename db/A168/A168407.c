/*
 * A168407  E.g.f.: Sum_{n>=0} (exp(2^n*x) - 1)^n/n!, an analog of the Bell
 *          numbers (A000110).
 * https://oeis.org/A168407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} S2(n,k) * 2^(n*k) where S2(n,k) are Stirling
 *            numbers of the second kind
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   pow2, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp); */
#include "../utils/seq_one.h"    /* seq_t seq_one(void); */

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return seq_one();

    /* Allocate array for Stirling numbers: s[k] holds S2(current_row, k) */
    seq_t *s = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!s)
        return (seq_t)0;

    s[0] = seq_one();   /* S2(0,0) = 1 */

    seq_t *t = NULL;    /* temporary pointer for current row updates */

    /* Compute Stirling numbers row by row, in-place from high k to low k */
    for (idx_t i = 1; i <= n; ++i) {
        /* Update in descending order to avoid overwriting needed values */
        for (idx_t k = i; k >= 1; --k) {
            s[k] = s[k - 1] + (seq_t)k * s[k];
        }
        s[0] = (seq_t)0;   /* S2(i,0) = 0 for all i > 0 */

        /* At the final row, compute the sum a(n) */
        if (i == n) {
            seq_t base   = pow2(i);            /* 2^n */
            seq_t power  = seq_one();
            seq_t sum    = (seq_t)0;

            for (idx_t k = 0; k <= i; ++k) {
                if (k > 0) {
                    power = power * base;  /* power = 2^(n*k) */
                }
                if (k > 0)
                    sum = sum + s[k] * power;
            }
            free(s);
            return sum;
        }
    }

    /* Should never reach here, but free memory just in case */
    free(s);
    return (seq_t)0;
}
