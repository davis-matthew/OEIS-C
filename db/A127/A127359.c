/*
 * A127359  a(n) = Sum_{k=0..n} binomial(n, floor(k/2))*3^(n-k).
 * https://oeis.org/A127359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,floor(k/2))*3^(n-k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"   /* declares seq_t binomial(idx_t, idx_t) */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;          /* accumulates the sum */

    /* Starting with k = n, 3^(n-k) = 1 */
    seq_t pow3 = (seq_t)1;

    for (idx_t k = n; ; --k) {
        idx_t j = k >> 1;             /* floor(k/2) */
        seq_t b = binomial(n, j);
        result += b * pow3;

        if (k == (idx_t)0)
            break;                   /* finished the loop, avoid underflow */
        pow3 *= (seq_t)3;            /* next power of three */
    }

    return result;
}
