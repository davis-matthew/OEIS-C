/*
 * A120798  a(n) = 3^(n^2)+2^n.
 * https://oeis.org/A120798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n^2)+2^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Compute n^2 as a seq_t to avoid overflow on very large n */
    seq_t sq = (seq_t)n * (seq_t)n;

    /* 3^(n^2) and 2^n using the provided fast exponentiation utilities */
    seq_t pow3n2 = pow3((idx_t)sq);
    seq_t pow2n   = pow2(n);

    return pow3n2 + pow2n;
}
