/*
 * A203611  a(n) = Sum_{k=0..n} binomial(k-1,2*k-1-n)*binomial(k,2*k-n), with
 *          a(0) = 1.
 * https://oeis.org/A203611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(k-1,2*k-1-n)*binomial(k,2*k-n),
 *            with a(0) = 1. See OEIS A203611
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 49, u128 n <= 95,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k)           /* k = 0 gives no valid term except n==0 */
    {
        /* compute the two binomial arguments in signed arithmetic */
        int64_t r1 = (int64_t)2 * (int64_t)k - 1 - (int64_t)n;
        int64_t r2 = (int64_t)2 * (int64_t)k - (int64_t)n;

        /* binomial(a,b) is 0 if b<0 or b>a; avoid unsigned wrap‑around */
        if (r1 < 0 || r1 > (int64_t)k - 1)
            continue;
        if (r2 < 0 || r2 > (int64_t)k)
            continue;

        seq_t b1 = binomial((idx_t)(k - 1), (idx_t)r1);
        if (b1 == 0)
            continue;
        seq_t b2 = binomial(k, (idx_t)r2);
        if (b2 == 0)
            continue;

        sum += b1 * b2;
    }

    return sum;
}
