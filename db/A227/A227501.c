/*
 * A227501  Number of non-congruent solutions of x^2 - xy + y^2 == 1 (mod n).
 * https://oeis.org/A227501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers of * a(p^e) = 2*3^e if p == 3 *
 *            a(p^e) = (p-1)*p^(e-1) if p % 3 == 1 * a(p^e) = (p+1)*p^(e-1) if
 *            p % 3 == 2 * * The sequence is multiplicative with the above
 *            prime power values
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t
a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but not used */
    seq_t result = 1;

    /* Case n == 1 is handled naturally: no primes, result stays 1 */

    /* Allocate buffers on stack. 64 factors are more than enough for
     * n up to 10^19 as 2*3*5*7*11*13*17*19 > 10^7. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t num_factors = 0;

    factor_u64((uint64_t)n, primes, exps, &num_factors);

    for (size_t i = 0; i < num_factors; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term;

        if (p == 3) {
            /* 2 * 3^e */
            term = 2 * pow_uint(3, (idx_t)e);
        } else if (p % 3 == 1) {
            /* (p-1) * p^(e-1) */
            term = (seq_t)(p - 1) * pow_uint(p, (idx_t)(e - 1));
        } else { /* p % 3 == 2 */
            /* (p+1) * p^(e-1) */
            term = (seq_t)(p + 1) * pow_uint(p, (idx_t)(e - 1));
        }

        result *= term;
    }

    return result;
}
