/*
 * A076936  a(1) = 1; then the smallest number different from its predecessor
 *          such that the n-th partial product is an n-th power.
 * https://oeis.org/A076936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^A014682(n-1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where A014682(n) is the minimal integer e such that the sum of the first n
 *  exponents is divisible by n, and e differs from the previous exponent.  */
/*  This implementation keeps only the running sum of exponents and the
 *  previous exponent, both of type idx_t.  The final term is 2 raised to
 *  the last exponent, obtained via the utility function pow2().
 */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n = 0 is outside the defined offset, return 0 as a placeholder */
    if (n == 0) return 0;

    idx_t sum     = 0;   /* sum of computed exponents so far */
    idx_t prev    = 0;   /* exponent of the previous term */
    idx_t e       = 0;   /* exponent for the current term */

    for (idx_t i = 1; i <= n; ++i) {
        if (i == 1) {
            e = 0;                  /* e_1 = 0  →  a(1) = 1 */
        } else {
            /* find the smallest e ≡ -sum (mod i) that is not equal to prev */
            idx_t mod     = sum % i;
            idx_t need    = (i - mod) % i;   /* desired residue modulo i */
            e = need;
            if (e == prev) {
                e += i;          /* skip the forbidden value */
            }
        }
        sum += e;
        prev = e;
    }

    /* return 2^prev as the nth term */
    return pow2(prev);
}
