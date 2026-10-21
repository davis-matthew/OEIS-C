/*
 * A233542  Number of ways to write n = k^2 + m with k > 0 and m > 0 such that
 *          phi(k^2)*phi(m) - 1 is prime, where phi(.) is Euler's totient
 *          function (A000010).
 * https://oeis.org/A233542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 1462, u32 n <= 1462, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, int_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the number of representations n = k^2 + m (k>0, m>0)
   such that phi(k^2)*phi(m) - 1 is a prime.
   a(n) = #{k, m ∈ ℕ : k>0, m>0, k^2 + m = n,
               is_prime(phi(k^2)*phi(m) - 1)} */
#include "../utils/calc_phi.h"
#include "../utils/is_prime_big.h"
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    if (n <= 5) return 0;          /* handle small n explicitly */
    idx_t maxk = int_sqrt(n - 1);  /* largest k with k^2 < n */
    for (idx_t k = 1; k <= maxk; ++k) {
        idx_t sq = k * k;
        idx_t m   = n - sq;
        seq_t phi_k2 = calc_phi((seq_t)sq);
        seq_t phi_m   = calc_phi((seq_t)m);
        seq_t val = phi_k2 * phi_m - 1;
        if (is_prime_big(val))
            ++cnt;
    }
    return cnt;
}
