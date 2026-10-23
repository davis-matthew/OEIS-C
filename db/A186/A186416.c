/*
 * A186416  a(n) = binomial(2n,n)^4/(n+1)^3.
 * https://oeis.org/A186416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(2n,n)^4 / (n+1)^3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* binomial(2n, n) */
    seq_t bin = binomial(n << 1, n);
    seq_t res = bin;
    seq_t divisor = (seq_t)(n + 1);
    /* need to divide by (n+1)^3 after the fourth power */
    idx_t rem = 3;
    for (idx_t i = 1; i < 4; ++i) {
        /* multiply by binomial again */
        res = res * bin;
        /* cancel as many factors of (n+1) as possible */
        while (rem > 0 && res % divisor == 0) {
            res = res / divisor;
            --rem;
        }
    }
    return res;
}
