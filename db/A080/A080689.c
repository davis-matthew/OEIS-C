/*
 * A080689  Powers of 10 that reach ...,7,8,4,2,1 under the mapping: if n is
 *          even divide by 2 else add 1.
 * https://oeis.org/A080689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth exponent k such that 10^k reaches 7 under the
 *            mapping f(x) = ceil(x/2). Equivalently, 6·2^t < 10^k ≤ 7·2^t for
 *            some integer t. This holds iff the fractional part of k·log₂10
 *            lies in the interval (log₂6 − ⎣log₂6⎦, log₂7 − ⎣log₂7⎦)
 * Range:     u8 n <= 55, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use long double for extra precision. */
    static const long double LOG2_10 = 3.321928094887362L;
    static const long double LOG2_6  = 2.584962500721156L;   /* ≈ log₂6   */
    static const long double LOG2_7  = 2.807354922057605L;   /* ≈ log₂7   */
    static const long double R1 = LOG2_6 - floor(LOG2_6);   /* ≈0.5849625 */
    static const long double R2 = LOG2_7 - floor(LOG2_7);   /* ≈0.8073549 */
    static const long double EPS = 1e-12L;                  /* safety margin */

    idx_t count = 0;
    seq_t term = 0;
    idx_t k = 1;

    while (count < n)
    {
        long double val = k * LOG2_10;
        long double frac = val - floor(val);
        if (frac > R1 + EPS && frac < R2 - EPS)
        {
            ++count;
            term = (seq_t)k;
            if (count == n)
                break;
        }
        ++k;
    }
    return term;
}
