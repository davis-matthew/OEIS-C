/*
 * A134353  Row sums of triangle A134352.
 * https://oeis.org/A134353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-2)*(2*n + 3 + (-1)^n) (valid for all n ≥ 0)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* compute 2^(n-2) */
    seq_t pow = pow2(n - 2);

    /* compute 2*n + 3 + (-1)^n  = 2*n + (n even ? 4 : 2) */
    seq_t two_n = (seq_t)n * (seq_t)2;
    seq_t add   = (n & (idx_t)1) ? (seq_t)2 : (seq_t)4; /* (-1)^n factor */
    seq_t term  = two_n + add;

    return pow * term;
}
