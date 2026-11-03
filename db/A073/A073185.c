/*
 * A073185  Sum of cubefree divisors of n.
 * https://oeis.org/A073185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sum of divisors of the cubefree kernel of n
 *            (equivalently, a(n) is multiplicative with a(p) = 1 + p a(p^e) =
 *            1 + p + p^2 for e > 1)
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 8942 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

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
    /* Undefined for n < 1; return 0 by convention */
    if (n < 1)
        return 0;

    /* a(1) = 1 */
    if (n == 1)
        return 1;

    /* Factor n; the helper works on uint64_t */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    _factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        if (exps[i] == 1)
            result *= (1 + p);            /* kernel exponent is 1 */
        else
        {
            seq_t p2 = p * p;              /* p^2 fits in seq_t */
            result *= (1 + p + p2);        /* kernel exponent is 2 */
        }
    }
    return result;
}
