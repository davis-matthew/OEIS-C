/*
 * A035798  Coordination sequence for lattice D*_26 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A035798: Coordination sequence for lattice D*_26 */
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Offset 0, a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    const idx_t N = 26;
    seq_t sum = 0;

    /* First summation part: k = 1 .. N
       (k=0 term vanishes for n>0) */
    for (idx_t k = 1; k <= N; ++k)
    {
        seq_t term = pow2(k);                     /* 2^k */
        term *= binomial(N, k);                   /* * C(N,k) */
        term *= binomial(n - 1, k - 1);           /* * C(n-1,k-1) */
        sum += term;
    }

    /* Second part: 2^N * C(n + 12, N-1) */
    seq_t second = pow2(N);                        /* 2^N */
    second *= binomial(n + 12, N - 1);            /* C(n+12,25) */

    return sum + second;
}
