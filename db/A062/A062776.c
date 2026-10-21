/*
 * A062776  Greatest common divisor of (n+2)! and n^n.
 * https://oeis.org/A062776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd((n+2)!, n^n)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */
#include <stddef.h>   /* for size_t */

/* ------------------------------------------------------------------ */
/* Simple trial‑division factorisation into primes.                 */
/* Returns number of distinct prime factors in *cnt.                  */
/* primes[i], exps[i] hold each prime and its exponent.              */
/* ------------------------------------------------------------------ */
static size_t
factor_u64(uint64_t n, uint64_t primes[], uint64_t exps[], size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            uint64_t e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            primes[c] = p;
            exps[c]   = e;
            ++c;
        }
    }
    if (n > 1)
    {
        primes[c] = n;
        exps[c]   = 1;
        ++c;
    }
    *cnt = c;
    return c;
}

/* ------------------------------------------------------------------ */
/* Efficient exponentiation by squaring for arbitrary seq_t values   */
/* ------------------------------------------------------------------ */
static seq_t
pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)1;   /* undefined for n < 1, return 1 as safe default */

    /* factorise n into primes */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t prime_cnt;
    factor_u64((uint64_t)n, primes, exps, &prime_cnt);

    seq_t result = (seq_t)1;
    uint64_t fact_limit = (uint64_t)n + 2;   /* upper bound for (n+2)! */

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p       = primes[i];
        uint64_t vp_n    = exps[i];              /* exponent of p in n   */
        uint64_t exp_n_n = vp_n * (uint64_t)n;   /* exponent of p in n^n */

        /* exponent of p in (n+2)! via Legendre's formula */
        uint64_t exp_fact = 0;
        uint64_t power = p;
        while (power <= fact_limit)
        {
            exp_fact += fact_limit / power;
            if (power > fact_limit / p) break;   /* avoid overflow */
            power *= p;
        }

        uint64_t exp = exp_n_n < exp_fact ? exp_n_n : exp_fact;
        if (exp == 0)
            continue;   /* no contribution from this prime */

        seq_t pow_val = pow_seq((seq_t)p, (idx_t)exp);
        result *= pow_val;
    }

    return result;
}
