/*
 * A110129  Central coefficients of a scaled Legendre triangle.
 * https://oeis.org/A110129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{j=0..floor(n/2)} (-1)^j*C(n, j)*C(2*n-2*j,
 *            n)*n^(n-2*j)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   binom, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow_seq.h"

/* fast exponentiation for seq_t; works for all build types */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    idx_t maxj = n >> 1;           /* floor(n/2) */
    seq_t result = 0;

    /* pre‑compute n^(n) and n^2 for division in the loop */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t cur   = pow_seq((seq_t)n, n);   /* cur = n^(n-2*j), j=0 */

    for (idx_t j = 0; j <= maxj; ++j) {
        seq_t bin1 = binom(n, j);                         /* C(n, j)          */
        seq_t bin2 = binom((idx_t)(2*n - 2*j), n);        /* C(2n-2j, n)      */

        seq_t term = bin1 * bin2 * cur;
        if ((j & 1) == 0)
            result += term;
        else
            result -= term;

        if (j != maxj)               /* prepare cur = n^(n-2(j+1)) */
            cur /= n_sq;
    }

    return result;
}
