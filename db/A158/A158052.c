/*
 * A158052  Largest integer power m (with 0 to stand for infinity) for which a
 *          representation of the form n = k^m exists (for some k >= 1)
 *          multiplied by -1 when k is prime.
 * https://oeis.org/A158052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = m * (-1)^{pi(k)-pi(k-1)}, where m is the largest exponent
 *            such that n = k^m, and the sign is negative iff k is prime
 *            (i.e., n is a perfect power of a prime). a(1)=0
 * Range:     i8 n <= 9721, i16 n <= 9721, i32 n <= 9721, i64 n <= 9721,
 *            i128 n <= 9721, big any n
 * Helpers:   mygcd_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mygcd_idx_t.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)0;   /* n = 1   → 0 (stands for infinity) */

    idx_t tmp = n;
    idx_t m = 0;              /* gcd of exponents */
    int   prime_count = 0;    /* number of distinct prime factors */
    idx_t exp;
    idx_t p;

    for (p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            ++prime_count;
            exp = 0;
            while (tmp % p == 0) {
                ++exp;
                tmp /= p;
            }
            if (m == 0)
                m = exp;
            else
                m = mygcd_idx_t(m, exp);
        }
    }
    if (tmp > 1) {   /* remaining prime factor */
        ++prime_count;
        exp = 1;
        if (m == 0)
            m = exp;
        else
            m = mygcd_idx_t(m, exp);
    }

    if (m == 0)   /* safety for n=1, already handled, but keep */
        m = 1;

    seq_t result = (seq_t)m;
    if (prime_count == 1)        /* n is a prime power (k prime) */
        result = -result;       /* negative sign */

    return result;
}
