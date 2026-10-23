/*
 * A135394  Number of walks of length 2*n+2 from origin to (1,1,0) on a cubic
 *          lattice.
 * https://oeis.org/A135394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = binomial(2*n+2, n)
 *                * Σ_{k=0..n} 
 *                  ( binomial(n, k) * binomial(n+2, k+1) * binomial(2*k+1, k) )
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t pref = binomial(2 * n + 2, n);
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = binomial(n, k);
        term *= binomial(n + 2, k + 1);
        term *= binomial(2 * k + 1, k);
        sum += term;
    }

    return pref * sum;
}
