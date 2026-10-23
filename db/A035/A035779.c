/*
 * A035779  Coordination sequence for C_42 lattice.
 * https://oeis.org/A035779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^42
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t m = 2 * n;                    /* exponent of x */
    seq_t total = 0;

    /* i ranges from 0 to the smaller of 42 and m */
    idx_t max_i = (m < 42 ? m : 42);

    for (idx_t i = 0; i <= max_i; ++i) {
        seq_t term = binomial(42, i);                /* C(42,i)     */
        term = term * binomial(41 + m - i, m - i);  /* C(41+m-i,m-i) */
        total += term;
    }
    return total;
}
