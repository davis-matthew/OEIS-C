/*
 * A173455  Row sums of triangle A027751.
 * https://oeis.org/A173455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of proper divisors of n, with a(1) = 1
 * Range:     u8 n <= 143, u16 n <= 4096, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, but return 0 */
    if (n == 1) return (seq_t)1;   /* custom offset */

    /* Factor n into primes */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exponents, &pcnt);

    /* Compute sigma(n) = product over primes of (p^(e+1)-1)/(p-1) */
    seq_t sigma = (seq_t)1;
    for (size_t i = 0; i < pcnt; ++i) {
        seq_t p  = (seq_t)primes[i];
        seq_t ep = (seq_t)exponents[i];

        seq_t p_pow = (seq_t)1;
        seq_t sum_p = (seq_t)1; /* includes p^0 */
        for (seq_t j = 1; j <= ep; ++j) {
            p_pow *= p;
            sum_p += p_pow;
        }
        sigma *= sum_p;
    }

    /* proper divisor sum = sigma(n) - n */
    seq_t result = sigma - (seq_t)n;
    return result;
}
