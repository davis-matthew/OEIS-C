/*
 * A151657  Number of permutations of 7 indistinguishable copies of 1..n with
 *          exactly 3 adjacent element pairs in decreasing order.
 * https://oeis.org/A151657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 120^n - (7*n + 1)*36^n + binomial(7*n+1, 2)*8^n -
 *            binomial(7*n+1, 3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, mul_small, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"
#include "../utils/binomial.h"
#include "../utils/mul_small.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Powers of 120, 36, and 8 */
    seq_t pow120 = seq_pow((seq_t)120, n);
    seq_t pow36  = seq_pow((seq_t)36,  n);
    seq_t pow8   = seq_pow((seq_t)8,   n);

    /* (7*n + 1) * 36^n */
    idx_t coef = 7 * n + 1;
    seq_t term2 = mul_small(pow36, (unsigned int)coef);

    /* binomial(7*n+1, 2) * 8^n */
    seq_t bin2    = binomial(7 * n + 1, 2);
    seq_t term3   = seq_mul(bin2, pow8);

    /* binomial(7*n+1, 3) */
    seq_t bin3 = binomial(7 * n + 1, 3);

    /* Combine all terms */
    seq_t value = pow120;
    value -= term2;
    value += term3;
    value -= bin3;

    return value;
}
