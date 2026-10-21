/*
 * A060899  Number of walks of length n on square lattice, starting at origin,
 *          staying on points with x+y >= 0.
 * https://oeis.org/A060899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * binomial(n, floor(n/2))
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   local_binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/local_binomial.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t k   = n >> 1;               /* floor(n/2) */
    seq_t bin = local_binomial(n, k); /* C(n, floor(n/2))   */
    /* 2^n: use left shift; works for all seq_t types that support it. */
    seq_t pow2 = ((seq_t)1) << n;

    return bin * pow2;
}
