/*
 * A052332  Number of labeled digraphs where every node has indegree 0 or
 *          outdegree 0 and no isolated nodes.
 * https://oeis.org/A052332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(n, k) * (2^k - 2)^(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses incremental powers of two and the identity   */
/* (2^0 - 2)^(n) = (-1)^n, which is added at the end of the sum.   */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Base case n = 0 */
    if (n == 0) {
        return 1;   /* a(0) = 1 */
    }

    seq_t result = 0;          /* running total of the sum for k ≥ 1 */
    seq_t power2 = 1;          /* 2^0 */

    /* Sum over k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k) {
        power2 *= 2;           /* now 2^k */
        seq_t base  = power2 - 2;              /* (2^k - 2) */
        seq_t exp   = (seq_t)(n - k);          /* exponent (n-k) */
        seq_t power;

        if (exp == 0) {        /* (anything)^0 = 1, also covers 0^0 */
            power = 1;
        } else {
            power = big_pow(base, exp);
        }

        seq_t bin = binomial(n, k);
        result += bin * power;
    }

    /* Add the k = 0 term: (-1)^n */
    if ((n & 1) == 0) {
        result += 1;           /* +1 for even n */
    } else {
        result -= 1;           /* -1 for odd n */
    }

    return result;
}
