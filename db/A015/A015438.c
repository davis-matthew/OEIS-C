/*
 * A015438  Gaussian binomial coefficient [ n,12 ] for q=-13.
 * https://oeis.org/A015438
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{i=1..12} ((-13)^(n-i+1)-1)/((-13)^i-1)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          Gaussian binomial coefficient [n,12] for q=-13, offset 12 */

#include "../utils/gcd.h"     /* provides seq_t gcd(seq_t a, seq_t b) */
#include "../utils/seq_pow.h" /* provides seq_t seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 12. For n < 12 we return 0, but
       callers normally do not use such indices. */
    if (n < 12) return 0;

    /* Pre‑compute the 12 denominator terms as positive integers.  For odd
       i we use |(-13)^i-1| = 13^i+1, for even i we use |(-13)^i-1| = 13^i-1. */
    seq_t den[12];
    for (idx_t i = 1; i <= 12; ++i) {
        seq_t p = seq_pow((seq_t)13, i);          /* 13^i   */
        den[i - 1] = (i & 1) ? (p + 1) : (p - 1); /* 13^i±1 */
    }

    seq_t result = 1;

    for (idx_t i = 1; i <= 12; ++i) {
        idx_t exp = n - i + 1;                     /* exponent for the numerator */
        seq_t p   = seq_pow((seq_t)13, exp);        /* 13^exp */
        seq_t num = (exp & 1) ? (p + 1) : (p - 1); /* |(-13)^exp-1| */

        /* Cancel common factors with the remaining denominators. */
        for (idx_t j = 0; j < 12; ++j) {
            seq_t g = gcd(num, den[j]);
            if (g > 1) {
                num /= g;
                den[j] /= g;
            }
        }

        /* Multiply the (now-fully-reduced) numerator into the running result. */
        result *= num;

        /* Cancel any remaining common factors between the new result and
           the denominators. */
        for (idx_t j = 0; j < 12; ++j) {
            seq_t g = gcd(result, den[j]);
            if (g > 1) {
                result /= g;
                den[j] /= g;
            }
        }
    }

    /* All denominators should now have reduced to 1; return the product. */
    return result;
}
