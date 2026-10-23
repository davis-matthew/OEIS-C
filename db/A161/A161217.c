/*
 * A161217  a(n) = Sum_{d|n} phi(n/d)^2*2^(d+1).
 * https://oeis.org/A161217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d)^2*2^(d+1)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   phi, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/phi.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(0) is defined as 0 */
    if (n == 0) {
        return 0;
    }

    seq_t total = 0;

    /* Iterate over all divisors d of n using i*i <= n */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* first divisor d = i */
            idx_t d = i;
            idx_t t = n / d;            /* n/d */
            seq_t ph = phi(t);          /* phi(n/d) */
            seq_t ph2 = ph * ph;        /* phi(n/d)^2 */
            seq_t pw = pow2(d + 1);     /* 2^(d+1) */
            total += ph2 * pw;

            /* second divisor if distinct */
            idx_t d2 = n / i;
            if (d2 != d) {
                idx_t t2 = n / d2;      /* n/d2 = i */
                seq_t ph2b = phi(t2) * phi(t2);
                seq_t pw2 = pow2(d2 + 1);
                total += ph2b * pw2;
            }
        }
    }

    return total;
}
