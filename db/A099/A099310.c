/*
 * A099310  Arithmetic derivative of Euler's totient function phi(n).
 * https://oeis.org/A099310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(A000010(n))
 * Range:     u8 n <= 96, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/phi.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    seq_t phi_val = phi(n);                       /* Euler's totient φ(n) */
    uint64_t phi64 = (uint64_t)phi_val;           /* φ(n) is ≤ n-1 (≤9999 for n≤10000) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(phi64, primes, exps, &cnt);        /* factor φ(n) */

    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t term = (phi_val / (seq_t)primes[i]) * (seq_t)exps[i];
        result += term;
    }
    return result;
}
