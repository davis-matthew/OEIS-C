/*
 * A128146  Numerators of Hankel transform of 1/J(n+1), where J(n)=A001045(n).
 * https://oeis.org/A128146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, frac
 * Formula:   a(n) = (-1)^C(n+1,2) * 2^(C(2*(n+1),3)/4)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 5, i128 n <= 6,
 *            big any n
 * Helpers:   binomial, pow_seq_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq_large.h"

seq_t a(idx_t n)
{
    /* Determine the sign: (-1)^C(n+1,2) where C(k,2)=k*(k-1)/2 */
    seq_t parity = (((seq_t)n * ((seq_t)n + 1) / 2) & 1);
    seq_t sign = (parity ? -1 : 1);

    /* Compute the exponent: C(2*(n+1),3)/4. The binomial is always divisible by 4. */
    idx_t two_n_plus_two = 2 * (n + 1);                /* 2*(n+1) */
    seq_t comb = binomial(two_n_plus_two, 3);          /* C(2*(n+1),3) */
    idx_t exponent = (idx_t)(comb / (seq_t)4);         /* integer division, fits in idx_t */

    /* Compute 2^exponent and apply the sign */
    seq_t pow2 = pow_seq_large((seq_t)2, (seq_t)exponent);
    return sign * pow2;
}
