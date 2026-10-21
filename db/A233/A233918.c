/*
 * A233918  a(n) = |{0 < k <= n/2: (phi(k) + phi(n-k))/2 is prime}|, where
 *          phi(.) is Euler's totient function.
 * https://oeis.org/A233918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 < k <= n/2 : (phi(k) + phi(n-k)) / 2 is prime}|
 * Range:     u8 n <= 819, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t kmax = n >> 1;           /* n / 2 */

    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t phik = phi(k);
        seq_t phin = phi(n - k);
        seq_t sum  = phik + phin;

        /* Count only when the sum is even (so the average is an integer) */
        if ((sum & 1) == 0) {
            seq_t mid = sum / 2;
            if (is_prime((idx_t)mid))
                ++count;
        }
    }

    return count;
}
