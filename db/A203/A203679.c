/*
 * A203679  Vandermonde sequence using x^2 + xy + y^2 applied to
 *          (1,2,4,...,2^(n-1)).
 * https://oeis.org/A203679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prod_{1<=i<j<=n} (x_i^2 + x_i*x_j + x_j^2) with
 *            x_i=2^{i-1} Equivalent: a(n) = 2^{n(n-1)(n-2)/3} * Π_{k=1}^{n-1}
 *            (1+2^k+2^{2k})^{n-k}
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used for offset 1 */
    /* exponent of the power of 2 */
    idx_t E = n * (n - 1) * (n - 2) / 3;
    seq_t res = seq_pow(2, E);

    seq_t two_k = 2;                /* 2^1 */
    for (idx_t k = 1; k <= n - 1; ++k)
    {
        seq_t f_val = 1 + two_k + two_k * two_k;  /* 1 + 2^k + 2^{2k} */
        idx_t exp = n - k;
        seq_t pow_f = seq_pow(f_val, exp);
        res = res * pow_f;
        two_k = two_k * 2;          /* prepare 2^(k+1) for next loop */
    }
    return res;
}
