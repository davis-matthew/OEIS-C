/*
 * A239615  a(n) = n * A239612(n) / A053191(n).
 * https://oeis.org/A239615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   for n≥1, let n=∏p_i^e_i. Then a(n)=∏ a(p_i^e_i), where - a(2^e)
 *            = 2^(e-1) * (3e + 1) - a(p^e) = p^(e-1) * ((p+1)e + p) if p≡1
 *            (mod 4) - a(p^e) = p^(e-1) * ((p-1)e + p) if p≡3 (mod 4) (p odd)
 *            a(1)=1
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    /* factor n using trial division */
    uint64_t up = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    if ((up & 1ULL) == 0ULL) {
        uint64_t exp = 0;
        while ((up & 1ULL) == 0ULL) {
            up >>= 1;
            ++exp;
        }
        primes[cnt] = 2ULL;
        exps[cnt]   = exp;
        ++cnt;
    }

    for (uint64_t d = 3ULL; d * d <= up; d += 2ULL) {
        if (up % d == 0ULL) {
            uint64_t exp = 0;
            while (up % d == 0ULL) {
                up /= d;
                ++exp;
            }
            primes[cnt] = d;
            exps[cnt]   = exp;
            ++cnt;
        }
    }

    if (up > 1ULL) {
        primes[cnt] = up;
        exps[cnt]   = 1ULL;
        ++cnt;
    }

    /* compute multiplicative value */
    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* compute p^(e-1) */
        seq_t powp = 1;
        for (uint64_t k = 1; k < e; ++k)
            powp *= (seq_t)p;

        seq_t coeff;
        if (p == 2ULL) {
            coeff = (seq_t)(3ULL * e + 1ULL);
        } else if ((p & 3ULL) == 1ULL) {          /* p ≡ 1 (mod 4) */
            coeff = (seq_t)((p + 1ULL) * e + p);
        } else {                                  /* p ≡ 3 (mod 4) */
            coeff = (seq_t)((p - 1ULL) * e + p);
        }

        result *= powp * coeff;
    }

    return result;
}
