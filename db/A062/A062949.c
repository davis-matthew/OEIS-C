/*
 * A062949  Multiplicative with a(p^e) = ((e+1)*p^(e+1)-(e+2)*p^e+1)/(p-1).
 * https://oeis.org/A062949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} phi(d)*tau(d) (Multiplicative; a(p^e) =
 *            ((e+1)*p^(e+1)-(e+2)*p^e+1)/(p-1)
 * Range:     u8 n <= 12, u16 n <= 2041, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9423 (2026-11-01)
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
    if (n == 1)          /* a(1) = 1 */
        return (seq_t)1;

    /* Factorize n.  _factor_u64 fills primes[], exps[], and cnt. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt;
    _factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* Compute p^e and p^(e+1) as seq_t. */
        seq_t p_pow_e  = (seq_t)1;
        for (uint64_t k = 0; k < e; ++k)
            p_pow_e *= p;
        seq_t p_pow_e1 = p_pow_e * p;

        /* numerator = (e+1)*p^(e+1) - (e+2)*p^e + 1 */
        seq_t numerator = (seq_t)(e + 1) * p_pow_e1
                         - (seq_t)(e + 2) * p_pow_e
                         + (seq_t)1;

        /* denominator = p - 1 (exact division). */
        seq_t part = numerator / (p - (seq_t)1);

        result *= part;
    }
    return result;
}
