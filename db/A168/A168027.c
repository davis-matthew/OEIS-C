/*
 * A168027  Noncomposite numbers in the southern ray of the Ulam spiral as
 *          oriented on the March 1964 cover of Scientific American.
 * https://oeis.org/A168027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*n^2 + 3*n + 1 for all n such that this number is prime
 *            * (a(0) = 1). The function enumerates n, tests each value for
 *            primality, * and returns the (n+1)-st qualifying number. The
 *            count is taken via a * linear scan; this is fast enough for the
 *            required bounds and works for * any of the sup...
 * Range:     u8 n <= 2, u16 n <= 26, u32 n <= 28, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t k = 0;           /* current n in the quadratic */
    idx_t cnt = 0;         /* number of qualifying terms seen */

    while (1) {
        /* compute 4*k^2 + 3*k + 1 in the appropriate seq_t type */
        seq_t kseq = (seq_t)k;
        seq_t val = ((seq_t)4) * kseq * kseq + ((seq_t)3) * kseq + 1;

        /* accept 1 (only for k==0) or a prime */
        if (k == 0 || is_prime_big(val)) {
            if (cnt == n)
                return val;
            ++cnt;
        }
        ++k;
    }
}
