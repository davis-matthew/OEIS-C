/*
 * A117659  Number of solutions to x^(k+2)=x^k mod n for some k>=1.
 * https://oeis.org/A117659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) is multiplicative. a(2^e) = 2^e (e < 3) or 2^(e-1)+4 (e ≥
 *            3) a(p^e) = p^(e-1)+2 (p > 2) a(1) = 1
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: factorise a uint64_t into arrays of primes and exponents.
   The function is only used for n ≤ 10000, so simple trial division
   is sufficient and keeps the code independent of external
   utilities. */
static void factor_uint64(uint64_t n,
                          uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    while ((n & 1ULL) == 0ULL) {          /* factor 2 */
        n >>= 1;
        if (c > 0 && pr[c-1] == 2ULL) ex[c-1]++; else {
            pr[c] = 2ULL; ex[c] = 1; c++;
        }
    }
    for (uint64_t f = 3; f * f <= n; f += 2) {
        while (n % f == 0) {
            n /= f;
            if (c > 0 && pr[c-1] == f) ex[c-1]++; else {
                pr[c] = f; ex[c] = 1; c++;
            }
        }
    }
    if (n > 1) {                         /* remaining prime */
        pr[c] = n; ex[c] = 1; c++;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* Store factorisation results.  For n ≤ 10000 at most 6 primes. */
    uint64_t primes[8];
    uint64_t exps[8];
    size_t pf_count = 0;
    factor_uint64((uint64_t)n, primes, exps, &pf_count);

    seq_t result = 1;

    for (size_t i = 0; i < pf_count; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e   = exps[i];
        seq_t factor;

        if (p == 2) {                    /* special case for 2^e */
            if (e < 3) {
                factor = ((seq_t)1ULL) << e;          /* 2^e */
            } else {
                factor = ((seq_t)1ULL) << (e - 1);    /* 2^(e-1) */
                factor = factor + 4;                 /* +4 */
            }
        } else {                         /* odd prime powers */
            seq_t pow = 1;
            for (uint64_t j = 1; j < e; ++j) {
                pow = pow * p;
            }
            factor = pow + 2;
        }
        result *= factor;
    }
    return result;
}
