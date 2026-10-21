/*
 * A239146  Smallest k>0 such that n +/- k and n^2 +/- k are all prime. a(n) =
 *          0 if no such number exists.
 * https://oeis.org/A239146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k>0 such that n±k and n^2±k are all prime;
 *            a(n)=0 if no such k exists
 * Range:     u8 n <= 291, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;          /* n=1 yields no valid k */
    uint64_t t  = (uint64_t)n;
    uint64_t n2 = t * t;

    for (uint64_t k = 1; k < n; ++k) {
        uint64_t n_k     = t - k;          /* n - k  */
        if (n_k < 2) continue;            /* must be prime */
        uint64_t n_k_plus  = t + k;        /* n + k  */
        uint64_t n2_k_minus = n2 - k;      /* n² - k */
        uint64_t n2_k_plus  = n2 + k;      /* n² + k */

        if (is_prime(n_k) &&
            is_prime(n_k_plus) &&
            is_prime(n2_k_minus) &&
            is_prime(n2_k_plus)) {
            return (seq_t)k;
        }
    }
    return (seq_t)0;
}
