/*
 * A120248  a(n) = Product_{k=0..n} C(n+k+2, n+2).
 * https://oeis.org/A120248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{k=0..n} C(n+k+2, n+2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use successive binomial recurrence to keep numbers small. */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t res = 1;        /* product accumulator */
    seq_t bin = 1;        /* C(n+2, n+2) = 1 for k = 0 */

    for (idx_t k = 0; k <= n; ++k)
    {
        res *= bin;       /* multiply by current binomial */

        if (k == n)
            break;        /* last iteration, no further binomial */

        /* Next binomial:
           C(n+k+3, n+2) = C(n+k+2, n+2) * (n+k+3)/(k+1)   */
        bin *= (seq_t)(n + k + 3);      /* numerator */
        bin /= (seq_t)(k + 1);          /* denominator */
    }

    return res;
}
