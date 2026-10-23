/*
 * A011787  Number of n X n matrices over Z_4 whose determinant is 1.
 * https://oeis.org/A011787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n^2-1) * ∏_{k=0}^{n-1} (2^n - 2^k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)           /* offset starts at 1, return 0 for safety */
        return 0;

    /* 2^(n^2-1) */
    idx_t exp = n * n - 1;
    seq_t res = pow2(exp);

    /* 2^n */
    seq_t pow2_n = pow2(n);

    /* multiply by each (2^n - 2^k) for k = 0 .. n-1 */
    for (idx_t k = 0; k < n; ++k) {
        seq_t pow2_k = pow2(k);
        seq_t term   = pow2_n - pow2_k;
        res *= term;
    }
    return res;
}
