/*
 * A172502  Denominator of fraction whose decimal representation has form
 *          0.(n)(n)(n)...with repeating part n.
 * https://oeis.org/A172502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (10^digits(n) - 1) / gcd(n, 10^digits(n) - 1)
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   digit_count, gcd, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_count.h"
#include "../utils/pow10.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Number of decimal digits of n */
    idx_t k = digit_count((uint64_t)n);

    /* Denominator before reduction: 10^k - 1 */
    seq_t denom = pow10(k);
    denom -= (seq_t)1;   /* equivalent to 10^k - 1 */

    /* Reduce by gcd with n */
    seq_t g = gcd((seq_t)n, denom);
    if (g != 0)
        denom /= g;

    return denom;
}
