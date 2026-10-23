/*
 * A191821  a(n) = n*(2^n - n + 1) + 2^(n-1)*(n^2 - 3*n + 2).
 * https://oeis.org/A191821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2^n - n + 1) + 2^(n-1)*(n^2 - 3*n + 2)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t for calculations */
    const seq_t ns = (seq_t)n;

    /* Precompute powers of 2 */
    const seq_t pow2n  = pow2(n);          /* 2^n   */
    const seq_t pow2n1 = pow2(n - 1);      /* 2^(n-1) */

    /* n^2 term */
    const seq_t n_sq = ns * ns;

    /* First part: n * (2^n - n + 1) */
    const seq_t part1 = ns * (pow2n - ns + 1);

    /* Second part: 2^(n-1) * (n^2 - 3n + 2) */
    const seq_t part2 = pow2n1 * (n_sq - 3 * ns + 2);

    return part1 + part2;
}
