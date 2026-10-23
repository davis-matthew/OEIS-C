/*
 * A116199  a(n) = the number of positive divisors of n which are coprime to
 *          sigma(n) = A000203(n).
 * https://oeis.org/A116199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive divisors d of n with gcd(d,σ(n)) = 1
 * Range:     u8 n <= 164, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* σ(n) is the sum of the positive divisors of n (OEIS A000203). */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but safe return */

    /* 1. Compute σ(n) */
    seq_t sigma = sigma_of(n);

    /* 2. Factor n: at most 16 distinct prime factors for 64‑bit n */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t prime_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &prime_cnt);

    /* 3. Generate all divisors of n */
    /* Maximum divisor count for n <= 10 000 is far below 2000, so this array is safe  */
    seq_t divs[4096];
    size_t dcnt = 1;
    divs[0] = (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        size_t cur = dcnt;          /* divisors known before processing this prime */
        seq_t p = (seq_t)primes[i];
        for (size_t j = 0; j < cur; ++j)
        {
            seq_t base = divs[j];
            seq_t val  = base;
            for (uint64_t k = 1; k <= exps[i]; ++k)   /* multiply by p^k and store */
            {
                val *= p;
                divs[dcnt++] = val;
            }
        }
    }

    /* 4. Count divisors coprime to σ(n) */
    seq_t count = 0;
    for (size_t i = 0; i < dcnt; ++i)
        if (gcd(divs[i], sigma) == (seq_t)1)
            ++count;

    return count;
}
