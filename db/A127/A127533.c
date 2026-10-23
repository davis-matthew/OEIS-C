/*
 * A127533  Sum of jump-lengths of all binary trees with n edges.
 * https://oeis.org/A127533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n+1, n-3) + binomial(2*n, n-3)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = a(1) = a(2) = 0 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    idx_t k = n - 3;
    seq_t term1 = binomial(2 * n + 1, k);
    seq_t term2 = binomial(2 * n,     k);
    return term1 + term2;
}
