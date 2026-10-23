/*
 * A152679  Even members of A000203, divided by 2.
 * https://oeis.org/A152679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1390, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = sigma(n)/2 for n with sigma(n) even, where sigma is the sum of divisors function */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Compute sigma(n) (sum of divisors) in type seq_t.
   Uses the multiplicative formula:
   sigma(p^e) = (p^(e+1)-1)/(p-1)   */
static seq_t sigma_u64(uint64_t n)
{
    seq_t sigma = 1;
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];

    factor_u64(n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* compute p^(e+1) */
        seq_t pp = 1;
        for (uint64_t j = 0; j < e + 1; ++j)
            pp *= (seq_t)p;

        /* term = (p^(e+1)-1)/(p-1) */
        seq_t denom = (seq_t)(p - 1);
        seq_t term = (pp - 1) / denom;

        sigma *= term;
    }

    return sigma;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, safe fallback */

    idx_t found = 0;
    uint64_t k = 2;                   /* start checking from n=2 */

    while (1) {
        seq_t sigma = sigma_u64(k);

        if ((sigma & 1) == 0) {      /* sigma even */
            ++found;
            if (found == n)
                return sigma >> 1;   /* divide by 2 */
        }

        ++k;
    }
}
