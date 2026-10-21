/*
 * A227528  a(n) = numerator of r(n) = 2*(3*n+2)!/((2*n)!*2^n), n>=0.
 * https://oeis.org/A227528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of r(n) = 2*(3*n+2)!/((2*n)!*2^n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   mul_small, pow2, seq_mul, tzcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_small.h"
#include "../utils/pow2.h"
#include "../utils/seq_mul.h"
#include "../utils/tzcount.h"

seq_t a(idx_t n)
{
    seq_t odd_part = 1;          /* product of the odd parts of the factors */
    idx_t exp_two  = 0;          /* total exponent of 2 in the product */

    idx_t start = 2 * n + 1;
    idx_t end   = 3 * n + 2;

    for (idx_t k = start; k <= end; ++k) {
        idx_t t = tzcount(k);           /* number of factors 2 in k */
        exp_two += t;
        idx_t odd_k = k >> t;           /* k with all 2's removed */
        odd_part = mul_small(odd_part, (unsigned int)odd_k);
    }

    /* leading factor 2 from the definition contributes one more 2 */
    exp_two += 1;

    /* net exponent of 2 after division by 2^n */
    idx_t net_twos = 0;
    if (exp_two > n)
        net_twos = exp_two - n;

    if (net_twos > 0) {
        seq_t pwr = pow2(net_twos);
        odd_part = seq_mul(odd_part, pwr);
    }

    return odd_part;
}
