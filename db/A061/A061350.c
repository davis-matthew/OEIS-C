/*
 * A061350  Maximal size of Aut(G) where G is a finite Abelian group of order
 *          n.
 * https://oeis.org/A061350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, nice, easy
 * Formula:   a(n) = ∏_{p^e‖n} ∏_{i=0}^{e-1} (p^e – p^i) (product over all
 *            prime–power divisors of n, GL(m,p) order for m=e)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 511,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)                 /* a(1) = 1, empty product otherwise */
        return 1;

    /* factor n into primes and their exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;

    uint64_t temp = (uint64_t)n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (temp > 1) {              /* remaining prime factor */
        primes[cnt] = temp;
        exps[cnt]   = 1;
        ++cnt;
    }

    seq_t result = 1;            /* multiplicative identity */

    for (size_t k = 0; k < cnt; ++k) {
        uint64_t p = primes[k];
        uint64_t e = exps[k];

        /* compute p^e once */
        seq_t p_pow_e = 1;
        for (uint64_t i = 0; i < e; ++i)
            p_pow_e *= (seq_t)p;

        /* compute ∏_{i=0}^{e-1} (p^e – p^i) */
        seq_t term = 1;
        seq_t p_pow_i = 1;          /* p^i, starts at i = 0 */
        for (uint64_t i = 0; i < e; ++i) {
            term *= (p_pow_e - p_pow_i);
            p_pow_i *= (seq_t)p;
        }

        result *= term;
    }

    return result;
}
