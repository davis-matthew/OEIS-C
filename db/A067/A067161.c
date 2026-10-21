/*
 * A067161  a(n) = prime(sigma(n)).
 * https://oeis.org/A067161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000040(A000203(n))
 * Range:     u8 n <= 16, u16 n <= 1978, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for non‑positive indices */

    /* Compute sigma(n): sum of divisors of n */
    uint64_t num = (uint64_t)n;
    size_t  cnt  = 0;
    uint64_t prime[128], exp[128];

    factor_u64(num, prime, exp, &cnt);

    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = prime[i];
        uint64_t e = exp[i];

        /* term = (p^(e+1) - 1)/(p - 1) */
        seq_t term_num = pow_uint(p, e + 1) - 1;
        seq_t term_den = (seq_t)(p - 1);
        sigma *= term_num / term_den;
    }

    /* sigma(1) = 1 is handled by the loop (cnt == 0) → sigma == 1 */

    /* Find the sigma(n)-th prime */
    idx_t prime_index = (idx_t)sigma;
    uint32_t prime_val = get_prime(prime_index);

    return (seq_t)prime_val;
}
