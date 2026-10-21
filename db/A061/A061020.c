/*
 * A061020  Negate primes in factorizations of divisors of n, then sum.
 * https://oeis.org/A061020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, mult
 * Formula:   a(n) = Σ_{d|n} d · λ(d) (λ = Liouville, λ(d)=(-1)^Ω(d))
 *            Equivalent multiplicative form: a(n) = ∏_{p^e‖n} (1 -
 *            (-p)^{e+1}) / (p + 1)
 * Range:     i8 n <= 12, i16 n <= 1408, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Simple trial‑division factoriser used only locally.  */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    *cnt = 0;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            pr[*cnt] = p;
            exp[*cnt] = e;
            ++(*cnt);
        }
    }
    if (m > 1) {
        pr[*cnt] = m;
        exp[*cnt] = 1;
        ++(*cnt);
    }
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    uint64_t nn   = (uint64_t)n;          /* index is unsigned */
    uint64_t pr[64];
    uint64_t e[64];
    size_t   pcnt;

    /* factorise n */
    factor_uint64(nn, pr, e, &pcnt);

    seq_t result = 1;                     /* multiplicative identity */

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p   = pr[i];
        uint64_t exp = e[i] + 1;           /* e+1 in formula */

        /* compute p_pow = p^(exp) as seq_t */
        seq_t p_pow = 1;
        for (uint64_t k = 0; k < exp; ++k)
            p_pow *= (seq_t)p;

        /* numerator = 1 - (-p)^{exp} */
        seq_t numer;
        if (exp & 1u)                     /* odd exponent → (-p)^exp = -p_pow   */
            numer = 1 + p_pow;             /* 1 - (-p_pow) = 1 + p_pow           */
        else
            numer = 1 - p_pow;             /* 1 -  p_pow                           */

        /* denominator = p + 1  (positive)  */
        seq_t denom = (seq_t)p + 1;

        /* exact division, integer result */
        seq_t term = numer / denom;

        /* accumulate multiplicatively */
        result = result * term;
    }

    return result;
}
