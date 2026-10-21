/*
 * A220790  Product(6^n - 6^k, k=0..n-1).
 * https://oeis.org/A220790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A220790: Product of (6^n - 6^k) for k = 0..n-1.
 * Formula: a(n) = ∏_{k=0}^{n-1} (6^n - 6^k)
 * This implementation uses the identity
 *   a(n) = 6^{n(n-1)/2} * ∏_{k=1}^{n} (6^k - 1).
 */
 
#include <stdint.h>                  /* for uint64_t (idx_t) */
#include "../utils/seq_pow.h"         /* seq_pow(base, exp)  */

/* External linkage function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 1;            /* a(0) = 1 */

    seq_t result = 1;                /* will hold ∏_{k=1}^{n} (6^k - 1)   */
    seq_t pow6   = 1;                /* running value of 6^k             */

    for (idx_t k = 1; k <= n; ++k)   /* accumulate the product of (6^k - 1) */
    {
        pow6 *= 6;                   /* pow6 = 6^k                       */
        result *= (pow6 - 1);        /* multiply by (6^k - 1)            */
    }

    /* multiply by 6^{n(n-1)/2} */
    idx_t exp2 = n * (n - 1) / 2;
    result *= seq_pow((seq_t)6, exp2);

    return result;
}
