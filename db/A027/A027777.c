/*
 * A027777  a(n) = 2*(n+1)*binomial(n+2,4).
 * https://oeis.org/A027777
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n+1)*binomial(n+2,4)
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                   /* below offset – behaviour irrelevant */
    seq_t bin = binomial(n + 2, 4);         /* C(n+2,4) */
    seq_t factor = (seq_t)(n + 1);          /* (n+1) */
    return (factor * 2) * bin;              /* 2*(n+1)*C(n+2,4) */
}
