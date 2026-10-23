/*
 * A035760  Coordination sequence for C_23 lattice.
 * https://oeis.org/A035760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^23
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 15, u128 n <= 123,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t twon = n + n;                       /* 2*n, unsigned wrap‑around is fine */
    idx_t max_k = twon < 23 ? twon : 23;      /* k runs from 0 to min(23,2*n) */
    seq_t total = 0;

    for (idx_t k = 0; k <= max_k; ++k)
    {
        seq_t c1 = binom(23, k);                          /* (1+x)^23 coefficient */
        seq_t c2 = binom(22 + (twon - k), 22);            /* (1-x)^(-23) coefficient */
        total += c1 * c2;
    }
    return total;
}
