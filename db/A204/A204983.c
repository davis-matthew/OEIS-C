/*
 * A204983  a(n) = 2^(k-1)-2^(j-1), where (2^(k-1),2^(j-1)) is the least pair
 *          of distinct positive powers of 2 for which n divides
 *          2^(k-1)-2^(j-1).
 * https://oeis.org/A204983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 2^A007814(n) * (2^A007733(n)-1) where A007814(n) =
 *            exponent of 2 in n and A007733(n) = multiplicative order of 2
 *            modulo the odd part of n
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   factor_u64, mod_pow, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow_uint.h"
#include "../utils/mod_pow.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset handling – not really needed */

    /* Extract the power of two factor of n. */
    idx_t v2   = 0;
    uint64_t tmp = (uint64_t)n;
    while ((tmp & 1ULL) == 0) {
        ++v2;
        tmp >>= 1;
    }
    uint64_t odd = tmp;        /* n/2^v2,  odd is >= 1 */

    /* Special case: odd part equals 1. */
    if (odd == 1) {
        seq_t pow_v2 = pow_uint(2, v2);
        return pow_v2;              /* 2^v2 * (2^1-1) == 2^v2 */
    }

    /* Compute phi(odd). */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(odd, primes, exps, &cnt);

    idx_t phi = odd;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t p = (idx_t)primes[i];
        phi = phi / p * (p - 1);
    }

    /* Factor phi to obtain its distinct prime divisors. */
    uint64_t phi_pr[64], phi_ex[64];
    size_t cntp = 0;
    factor_u64(phi, phi_pr, phi_ex, &cntp);

    /* Compute the multiplicative order of 2 mod odd. */
    idx_t order = phi;
    for (size_t i = 0; i < cntp; ++i) {
        idx_t p = (idx_t)phi_pr[i];
        while (order % p == 0 &&
               mod_pow((seq_t)2, (idx_t)(order / p), (seq_t)odd) == (seq_t)1)
        {
            order /= p;
        }
    }

    /* Assemble the result: 2^v2 * (2^order - 1). */
    seq_t pow_v2 = pow_uint(2, v2);
    seq_t pow_o  = pow_uint(2, order);

    return pow_v2 * (pow_o - (seq_t)1);
}
