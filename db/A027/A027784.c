/*
 * A027784  a(n) = 11*(n+1)*binomial(n+2,11)/2.
 * https://oeis.org/A027784
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+1,10) * binomial(n+2,2)
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 26, u64 n <= 153,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(n) is defined for n ≥ 9; for n < 9 the value is undefined in OEIS */
    if (n < 9u) return 0;

    /* compute C(n+1,10) and C(n+2,2) using the provided binomial helper */
    seq_t c1 = binomial(n + 1u, 10u);   /* C(n+1,10) */
    seq_t c2 = binomial(n + 2u, 2u);    /* C(n+2,2) */

    return c1 * c2;   /* exact multiplication in seq_t */
}
