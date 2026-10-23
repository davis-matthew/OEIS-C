/*
 * A105119  Numbers obtained by rotating right the indices in the prime
 *          signature of n.
 * https://oeis.org/A105119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product p_i^{e_{i-1}} where n = ∏ p_i^{e_i} with primes
 *            in ascending order and indices are rotated right by one position
 *            (e_0 receives e_k)
 * Range:     u8 n <= 51, u16 n <= 271, u32 n <= 2367, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint64.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;                 /* Special case: 1 has no primes */
    idx_t tmp = n;
    /* There are at most 64 distinct prime factors for 64‑bit numbers */
    idx_t  primes[64];
    uint64_t exps[64];
    int m = 0;                             /* Number of distinct primes */

    for (idx_t p = 2; p <= tmp / p; ++p) { /* trial division without overflow */
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[m] = p;
            exps[m]   = e;
            ++m;
        }
    }
    if (tmp > 1) {                         /* Remaining prime factor */
        primes[m] = tmp;
        exps[m]   = 1;
        ++m;
    }

    seq_t result = 1;
    for (int i = 0; i < m; ++i) {
        uint64_t e_rot = (i == 0) ? exps[m - 1] : exps[i - 1];
        seq_t term = pow_uint64(primes[i], e_rot);
        result *= term;
    }
    return result;
}
