/*
 * A057918  Number of pairs of numbers (r,s) each less than n such that
 *          (r,s,n) is in geometric progression.
 * https://oeis.org/A057918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{k ∈ [1,n-1] | n*k is a perfect square}
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;   /* a(1) = 0 */

    /* Factor n to obtain the square‑free part s */
    uint64_t temp = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(temp, primes, exps, &pcnt);

    seq_t s = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        if ((exps[i] & 1ULL) != 0) {
            s *= (seq_t)primes[i];
        }
    }

    /* Count w: w^2 < n/s  →  floor((n-1)/s) square root */
    seq_t t = (seq_t)(n - 1) / s;
    return floor_sqrt(t);
}
