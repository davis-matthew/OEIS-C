/*
 * A053347  a(n) = binomial(n+7, 7)*(n+4)/4.
 * https://oeis.org/A053347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 51, u64 n <= 879, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Formula: a(n) = binomial(n+7,7)*(n+4)/4 */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* compute C(n+7,7) */
    seq_t bin = binomial(n + 7, 7);      /* seq_t */
    seq_t mul = n + 4;                   /* seq_t */

    /* divide the total product by 4, performing the division in two
       steps to avoid overflow (one factor divisible by 2 at each step). */
    for (int i = 0; i < 2; ++i) {
        if ((bin & 1) == 0)          /* bin is even */
            bin >>= 1;               /* bin /= 2 */
        else
            mul >>= 1;               /* mul /= 2 */
    }

    return bin * mul;                  /* exact result */
}
