/*
 * A068324  Number of nondecreasing arithmetic progressions of positive odd
 *          integers with sum n.
 * https://oeis.org/A068324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of nondecreasing arithmetic progressions of
 *            positive odd integers whose sum is n. The implementation
 *            enumerates all divisors k of n. For each k, let q = n / k. The
 *            sum constraint leads to the counting formula described in the
 *            analysis: - If k = 1: contribute 1 if q is odd. - If k is...
 * Range:     u8 n <= 779, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _count_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_count_k.h"

seq_t a(idx_t n)
{
    seq_t ans = 0;

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t k1 = i;
            idx_t q1 = n / k1;
            ans += _count_k(k1, q1);

            idx_t k2 = n / i;
            if (k2 != k1) {
                idx_t q2 = n / k2;   /* which equals i */
                ans += _count_k(k2, q2);
            }
        }
    }

    return ans;
}
