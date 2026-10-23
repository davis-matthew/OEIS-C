/*
 * A035746  Coordination sequence for C_9 lattice.
 * https://oeis.org/A035746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^9 (equivalently a(n) = Σ_{k=0}^{9}
 *            binom(9,k)*binom(8+2n-k,8) ) Returns the nth term of OEIS
 *            A035746
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 13, u64 n <= 220,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"

/* Computes a(n) for any n >= 0. */
seq_t a(idx_t n)
{
    idx_t twon = n << 1;      /* 2*n, safe for idx_t 64-bit */
    seq_t total = 0;

    for (idx_t k = 0; k <= 9; ++k) {
        if (twon < k) break;                      /* binomial(top,8) = 0 for top < 8 */
        seq_t c1 = binomial(9, k);                /* binom(9,k) */
        idx_t top = 8 + twon - k;                  /* 8 + 2n - k */
        seq_t c2 = binomial(top, 8);               /* binom(8+2n-k,8) */
        total += c1 * c2;                          /* accumulation */
    }
    return total;
}
