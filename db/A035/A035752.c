/*
 * A035752  Coordination sequence for C_15 lattice.
 * https://oeis.org/A035752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^15
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 34, u128 n <= 813,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* 2*n may overflow when n is large, but idx_t is 64‑bit and n<=10000 here */
    idx_t twice = (idx_t)2 * n;
    seq_t result = 0;

    /* k runs from 0 to min(15, 2*n) */
    idx_t kmax = 15;
    if (twice < kmax) kmax = twice;

    for (idx_t k = 0; k <= kmax; ++k)
    {
        /* binom(15, k) */
        seq_t c1 = binom(15, k);

        /* remaining exponent */
        idx_t rem = twice - k;

        /* binom(14 + rem, rem) */
        seq_t c2 = binom(14 + rem, rem);

        result += c1 * c2;
    }

    return result;
}
