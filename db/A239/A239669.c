/*
 * A239669  Total number of prime factors counted with multiplicity of
 *          prime(n)-1 and prime(n)+1, where prime(n) is the n-th prime.
 * https://oeis.org/A239669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Omega(prime(n)^2 - 1) (sum of prime factors with
 *            multiplicity of prime(n)-1 and prime(n)+1)
 * Range:     u8 n <= 54, u16 n <= 6954, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    /* prime(n) is the n-th prime; n is indexed from 1 */
    uint64_t p = get_prime(n);           /* get n-th prime (uint32_t output, cast to 64 for safety) */

    seq_t omega_minus = count_prime_factors((seq_t)(p - 1));   /* Omega(p-1) */
    seq_t omega_plus  = count_prime_factors((seq_t)(p + 1));   /* Omega(p+1) */

    return omega_minus + omega_plus;      /* Omega(p^2-1) = Omega(p-1)+Omega(p+1) */
}
