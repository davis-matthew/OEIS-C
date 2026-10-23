/*
 * A197194  a(n) = binomial(n+9, 9)*9^n.
 * https://oeis.org/A197194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+9, 9) * 9^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 31,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute binomial(n+9, 9) multiplicatively to avoid factorials. */
    seq_t binom = 1;
    for (idx_t i = 1; i <= 9; ++i) {
        binom *= (seq_t)(n + i);   /* (n+i) fits in seq_t when cast */
        binom /= (seq_t)i;         /* exact division */
    }

    /* Compute 9^n using the utility big_pow (works for seq_t all sizes). */
    seq_t pow9 = big_pow((seq_t)9, (seq_t)n);

    /* Return the product.  For small n the result fits in seq_t without
       overflow; for larger n the result may overflow the type, but
       values that fit will be correct. */
    return binom * pow9;
}
