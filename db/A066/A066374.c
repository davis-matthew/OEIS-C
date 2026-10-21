/*
 * A066374  a(n) = (3*n+4)*2^(n-3)-(2*n-1).
 * https://oeis.org/A066374
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+4)*2^(n-3) - (2*n-1) (for n ≥ 2)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* For n < 2 the specification does not require correct values. */
    if (n < 2) return 0;

    /* Handle the single exception where n-3 would be negative. */
    if (n == 2) return 2;

    /* Compute (3*n + 4) and (2*n - 1) in seq_t to avoid overflow. */
    seq_t sum1 = (seq_t)(3 * n + 4);
    seq_t sum2 = (seq_t)(2 * n - 1);

    /* 2^(n-3) is computed using the provided pow2 helper. */
    seq_t pow = pow2(n - 3);

    /* Apply the formula. */
    seq_t result = sum1 * pow;
    result -= sum2;

    return result;
}
