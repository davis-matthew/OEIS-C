/*
 * A062803  Number of solutions to x^2 == y^2 (mod n).
 * https://oeis.org/A062803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) is multiplicative. For an odd prime power p^e: a(p^e) =
 *            ((p-1)*e + p) * p^(e-1). For a power of two: a(2^e) = e * 2^e.
 *            (See OEIS A062803 for details.)
 * Range:     u8 n <= 47, u16 n <= 2879, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9981 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* ---------- Trial division factorisation ----------------------------------- */
static void _factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    *cnt = 0;
    if (n < 2) return;
    /* factor 2 separately */
    if ((n & 1ULL) == 0) {
        pr[*cnt] = 2ULL;
        exp[*cnt] = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            exp[*cnt] += 1;
        }
        (*cnt)++;
    }
    /* odd primes */
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0) {
            pr[*cnt] = d;
            exp[*cnt] = 0;
            while (n % d == 0) {
                n /= d;
                exp[*cnt] += 1;
            }
            (*cnt)++;
        }
    }
    if (n > 1) {
        pr[*cnt] = n;
        exp[*cnt] = 1;
        (*cnt)++;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1, return 0 */
    if (n == 1) return (seq_t)1;         /* a(1) = 1 */

    /* factor n */
    uint64_t pr[64];
    uint64_t exp[64];
    size_t cnt = 0;
    _factor_u64((uint64_t)n, pr, exp, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term;
        if (pr[i] == 2ULL) {                 /* a(2^e) = e * 2^e */
            unsigned long long e = exp[i];
            seq_t pow2 = (seq_t)1;
            for (unsigned long long j = 0; j < e; ++j)
                pow2 <<= 1;                   /* 2^e */
            term = (seq_t)e * pow2;
        } else {                             /* odd prime power */
            seq_t p  = (seq_t)pr[i];
            seq_t e  = (seq_t)exp[i];

            /* compute p^(e-1) */
            seq_t p_pow = (seq_t)1;
            if (exp[i] > 1) {
                for (unsigned long long j = 1; j < exp[i]; ++j)
                    p_pow *= p;
            }

            /* coeff = ((p-1)*e + p) */
            seq_t coeff = ((seq_t)(pr[i] - 1)) * e + p;

            term = coeff * p_pow;
        }
        result *= term;
    }
    return result;
}
