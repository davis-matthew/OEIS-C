/*
 * A234200  a(n) = |{0 < k < n/2: k*phi(n-k) - 1 and k*phi(n-k) + 1 are both
 *          prime}|, where phi(.) is Euler's totient function.
 * https://oeis.org/A234200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 < k < n/2 : k*phi(n-k) - 1 and k*phi(n-k) + 1 are
 *            both prime}|
 * Range:     u8 n <= 33, u16 n <= 508, u32 n <= 1593, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#include "../utils/calc_phi.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t max_k = (n - 1) / 2;           /* largest integer k with 0 < k < n/2 */

    for (idx_t k = 1; k <= max_k; ++k) {
        seq_t m = (seq_t)n - k;          /* n - k */
        seq_t phi_val = calc_phi(m);
        seq_t prod = (seq_t)k * phi_val;
        seq_t a1 = prod - 1;
        seq_t a2 = prod + 1;

        if (is_prime_big(a1) && is_prime_big(a2))
            ++count;
    }
    return count;
}
