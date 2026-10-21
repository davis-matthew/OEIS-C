/*
 * A208570  LCM of n and smallest nondivisor of n.
 * https://oeis.org/A208570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n, smallest nondivisor of n)
 * Range:     u8 n <= 59, u16 n <= 7559, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest nondivisor of n is the smallest integer >1 that does not divide n.
   For odd n that is 2, and for even n it is the first k>=3 with n%k!=0.  */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Find smallest integer k > 1 such that n % k != 0 */
    idx_t k = 2;
    for (; ; ++k) {
        if (n % k != 0) break;
    }

    /* Compute lcm(n, k) = n / gcd(n, k) * k using seq_t arithmetic */
    seq_t n_s  = (seq_t)n;
    seq_t k_s  = (seq_t)k;
    seq_t g    = gcd(n_s, k_s);
    seq_t res  = (n_s / g) * k_s;
    return res;
}
