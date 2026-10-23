/*
 * A172504  a(n) = denominator of fraction a/b, where gcd(a, b) = 1, whose
 *          decimal representation has the form (n).(n)(n)(n)... with period
 *          (n).
 * https://oeis.org/A172504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (10^{digits(n)} - 1) / gcd(n, 10^{digits(n)} - 1)
 * Range:     u8 n <= 27, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   gcd, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute the number of decimal digits of n */
    idx_t k = 0;
    idx_t t = n;
    while (t > 0) { ++k; t /= 10; }
    if (k == 0) k = 1; /* for completeness, n≥1 so this is unused */

    /* 10^k - 1 */
    seq_t pow10k = pow10(k);
    seq_t denom   = pow10k - 1;

    /* Reduce fraction n*10^k / (10^k-1) */
    seq_t g = gcd((seq_t)n, denom);
    return denom / g;
}
