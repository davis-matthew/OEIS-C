/*
 * A173187  a(n) = binomial(n + 3, 3)*9^n.
 * https://oeis.org/A173187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n + 3, 3) * 9^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 36,
 *            big any n
 * Helpers:   binomial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* binomial(n + 3, 3) gives (n+3)(n+2)(n+1)/6 */
    seq_t binom = binomial(n + 3, 3);
    /* 9^n */
    seq_t p9 = pow_int((seq_t)9, n);
    /* product */
    return binom * p9;
}
