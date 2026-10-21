/*
 * A077080  a(n) = phi(sigma(n) + phi(n)) = A000010(A000203(n) + A000010(n)) =
 *          A000010(A065387(n)).
 * https://oeis.org/A077080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077080: a(n) = φ(σ(n) + φ(n))  
   φ is Euler's totient, σ is the sum of divisors.  */

/* For the required table we compute σ(n) and φ(n) from the prime
   factorisation of n.  In every build we have a helper
   factor_u64 that fills two parallel arrays of primes and exponents.
   All arithmetic is done in 64‑bit unsigned integers; the final
   totient is cast to the build's seq_t type. */

#include <stdint.h>
#include "../utils/factor_u64.h"

/* Compute σ(n) = product over p^e of (p^(e+1)-1)/(p-1) */
static uint64_t sigma_u64(uint64_t n)
{
    uint64_t primes[64], exps[64];
    size_t cnt;

    factor_u64(n, primes, exps, &cnt);

    uint64_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t term = 1;
        for (uint64_t e = 0; e < exps[i]; ++e) {
            term = term * primes[i] + 1;
        }
        result *= term;
    }
    return result;
}

/* Compute φ(n) = product over p^e of p^(e-1)*(p-1) */
static uint64_t phi_u64(uint64_t n)
{
    uint64_t primes[64], exps[64];
    size_t cnt;

    factor_u64(n, primes, exps, &cnt);

    uint64_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t term = 1;
        for (uint64_t e = 1; e < exps[i]; ++e) {
            term *= primes[i];
        }
        term *= (primes[i] - 1);
        result *= term;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;
    uint64_t phi_n   = phi_u64(n64);
    uint64_t sigma_n = sigma_u64(n64);
    uint64_t sum     = sigma_n + phi_n;
    uint64_t phi_sum = phi_u64(sum);
    return (seq_t)phi_sum;
}
