/*
 * A233544  Number of ways to write n = k^2 + m with k > 0 and m >= k^2 such
 *          that sigma(k^2) + phi(m) is prime, where sigma(k^2) is the sum of
 *          all (positive) divisors of k^2, and phi(.) is Euler's totient
 *          function (A000010).
 * https://oeis.org/A233544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(k,m): k>0, m≥k², n = k² + m, and σ(k²)+φ(m) is prime}|
 * Range:     u8 n <= 881, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, phi, sigma_of_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/is_prime.h"
#include "../utils/sigma_of_n.h"

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t k = 1;; ++k) {
        idx_t k2 = k * k;
        if (k2 > n)          /* k² alone exceeds n */
            break;
        idx_t m = n - k2;
        if (m < k2)          /* further k will only make m smaller */
            break;
        seq_t sigma = sigma_of_n(k2);
        seq_t phi_m  = phi(m);
        seq_t tot    = sigma + phi_m;
        if (is_prime((idx_t)tot))
            ++cnt;
    }
    return cnt;
}
