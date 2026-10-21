/*
 * A050458  Difference between Sum_{d|n, d == 1 (mod 4)} d^2 and Sum_{d|n, d
 *          == 3 (mod 4)} d^2.
 * https://oeis.org/A050458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   A050458(n) = prod_{p^e||n, p>2} a(p^e) where a(2^e)=1,
 *            a(p^e)=((p^2)^(e+1)-1)/(p^2-1) if p≡1 (mod 4),
 *            a(p^e)=((p^2)^(e+1)+(-1)^e)/(p^2+1) if p≡3 (mod 4)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 306, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* multiply two seq_t values; the underlying arithmetic type can be
   8/16/32/64 bits or arbitrary precision; here we use the normal
   multiplication operator which the build environment guarantees to
   work for the chosen type. */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Factorise n (trial division is sufficient for n up to ~10⁴). */
    uint64_t m = (uint64_t)n;
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;

    /* factor 2 */
    if ((m & 1ULL) == 0) {
        uint64_t e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++e;
        }
        primes[cnt] = 2;
        exps[cnt]   = e;
        ++cnt;
    }

    /* factor odd part */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (m > 1) {                /* remaining prime factor */
        primes[cnt] = m;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* compute the multiplicative product of the prime‑power contributions */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* p==2 contributes 1; skip it */
        if (p == 2)
            continue;

        seq_t p2 = (seq_t)p * (seq_t)p;
        seq_t contrib;

        if (p % 4 == 1) {                    /* p ≡ 1 mod 4 */
            seq_t num = pow_seq(p2, e + 1) - 1;
            seq_t den = p2 - 1;
            contrib = num / den;
        } else {                              /* p ≡ 3 mod 4 */
            seq_t num = pow_seq(p2, e + 1);
            if ((e & 1ULL) == 0)
                num += 1;                     /* (-1)^e = +1 for even e */
            else
                num -= 1;                     /* (-1)^e = -1 for odd e  */
            seq_t den = p2 + 1;
            contrib = num / den;
        }
        result *= contrib;
    }

    return result;
}
