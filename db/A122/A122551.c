/*
 * A122551  Denominators of the coefficients of the series for InverseErf(x).
 * https://oeis.org/A122551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n+1)!*2^(n+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_one.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* Compute the denominators of the coefficients of the series for InverseErf(x) */
seq_t a(idx_t n)
{
    /* compute (2*n+1)! */
    seq_t fac = seq_one();                     /* start with 1 */
    idx_t limit = 2*n + 1;                     /* 2*n+1 fits in idx_t for intended use */
    for (idx_t i = 2; i <= limit; ++i)
        fac = seq_mul(fac, (seq_t)i);          /* fac *= i */

    /* compute 2^(n+1) */
    seq_t pow2 = seq_pow((seq_t)2, n + 1);

    /* result = fac * pow2 */
    return seq_mul(fac, pow2);
}
