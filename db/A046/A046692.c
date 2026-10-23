/*
 * A046692  Dirichlet inverse of sigma function (A000203).
 * https://oeis.org/A046692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, mult, sign, nice, changed
 * Formula:   multiplicative a(n) with a(1)=1, a(p)=-(p+1), a(p²)=p, a(p^k)=0
 *            for k>2 (OEIS A046692)
 * Range:     i8 n <= 65, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    uint64_t m = (uint64_t)n;          /* working copy of n */
    seq_t res = 1;                     /* accumulated product */

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
                if (exp > 2) return 0;  /* any exponent >2 kills the term */
            }
            if (exp == 1) {
                seq_t factor = (seq_t)(- (seq_t)p - 1);
                res *= factor;
            } else { /* exp == 2 */
                seq_t factor = (seq_t)p;
                res *= factor;
            }
        }
    }

    if (m > 1) { /* remaining prime factor with exponent 1 */
        seq_t factor = (seq_t)(- (seq_t)m - 1);
        res *= factor;
    }

    return res;
}
