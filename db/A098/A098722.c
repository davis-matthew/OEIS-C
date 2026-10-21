/*
 * A098722  a(n) = C(n, 3)^(n-4).
 * https://oeis.org/A098722
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n,3)^(n-4) for n >= 3
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 3.  Behaviour for
       n < 3 is outside the required range; returning 0 is safe
       without impacting correctness for allowed indices. */
    if (n < 3)
        return 0;

    /* Compute the binomial coefficient C(n,3). */
    seq_t base = binomial(n, 3);

    /* Exponent is n-4.  Convert to seq_t to match big_pow's
       parameter type. */
    seq_t exp = (seq_t)(n - 4);

    /* Return base raised to exp.  big_pow handles arbitrary
       precision when seq_t is an arbitrary‑precision class. */
    return big_pow(base, exp);
}
