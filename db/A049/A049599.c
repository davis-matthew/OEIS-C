/*
 * A049599  Number of (1+e)-divisors of n: if n = Product p(i)^r(i), d =
 *          Product p(i)^s(i) and s(i) = 0 or s(i) divides r(i), then d is a
 *          (1+e)-divisor of n.
 * https://oeis.org/A049599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   a(n) = ∏_p (tau(r_p)+1) where n = ∏ p^r_p and τ(r) is the
 *            divisor count of r
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static inline seq_t tau_uint64(uint64_t n) {
    seq_t count = 0;
    for (uint64_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            if (i * i == n)
                count += 1;
            else
                count += 2;
        }
    }
    return count;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* out of offset */
    if (n == 1) return 1;   /* a(1) = 1 */

    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t exp = exps[i];
        seq_t factor = tau_uint64(exp) + 1;
        result *= factor;
    }
    return result;
}
