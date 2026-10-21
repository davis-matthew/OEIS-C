/*
 * A080647  Sum of prime factors of phi(n).
 * https://oeis.org/A080647
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct prime factors of phi(n)
 * Range:     u8 n <= 413, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n < 3) {
        return 0;
    }

    /* Compute phi(n) */
    seq_t ph = phi(n);

    /* Factor phi(n) and sum distinct prime factors */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    uint64_t phi64 = (uint64_t)ph;            /* phi fits in 64 bits for n <= 10000 */
    factor_u64(phi64, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        sum += (seq_t)primes[i];
    }

    return sum;
}
