/*
 * A132960  a(n) = n!*Sum_{d|n} (-1)^(d+1)/(d!*(n/d)^d).
 * https://oeis.org/A132960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n!*Sum_{d|n} (-1)^(d+1)/(d!*(n/d)^d)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses integer prime‑factor arithmetic and the
   square‑free prime list up to the current n.  */
#include <stdint.h>
#include <stdlib.h>

/* Utilities provided by the build system */
#include "../utils/big_pow.h"       /* seq_t big_pow(seq_t base, seq_t exp);  */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t limit,
                                               uint64_t **primes_out,
                                               size_t *cnt_out);   */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n,
                                         uint64_t *primes,
                                         uint64_t *exps,
                                         size_t *cnt);        */

/* Static cache of primes up to the largest n we have seen so far.  */
static uint64_t *prime_list = NULL;  /* actual prime values */
static size_t   prime_cnt   = 0;     /* number of primes in prime_list */
static idx_t    prime_cap   = 0;     /* maximal n served so far */

/* Ensure that prime_list contains all primes <= n.  */
static void ensure_primes(idx_t n)
{
    if (prime_cap >= n) return;          /* already have enough primes */

    free(prime_list);                    /* release old array */
    size_t cnt;
    build_primes_upto(n, &prime_list, &cnt); /* primes up to n, inclusive */
    prime_cnt = cnt;
    prime_cap = n;
}

/* Legendre exponent of prime p in m!   */
static idx_t exp_in_factorial(idx_t m, uint64_t p)
{
    idx_t e = 0;
    while (m)
    {
        m /= p;
        e += m;
    }
    return e;
}

/* Exponent of p in the integer x (x>=1).  */
static idx_t exp_in_number(idx_t x, uint64_t p)
{
    idx_t e = 0;
    while (x % p == 0)
    {
        x /= p;
        ++e;
    }
    return e;
}

/* Recursive divisor generator: pos is the current prime index.  */
static void gen_div_rec(idx_t prod, size_t pos,
                        const uint64_t *primes, const uint64_t *exps,
                        size_t cnt, idx_t **out, size_t *len, size_t *cap)
{
    if (pos == cnt)
    {
        /* Append prod to output array.  */
        if (*len + 1 > *cap)
        {
            *cap = (*cap == 0) ? 4 : (*cap << 1);
            *out = (idx_t *)realloc(*out, *cap * sizeof(idx_t));
        }
        (*out)[(*len)++] = prod;
        return;
    }

    /* Multiply by p^e for e = 0 .. exps[pos] */
    uint64_t p = primes[pos];
    idx_t mult = 1;
    for (idx_t e = 0; e <= (idx_t)exps[pos]; ++e)
    {
        gen_div_rec(prod * mult, pos + 1, primes, exps, cnt, out, len, cap);
        mult *= (idx_t)p;
    }
}

/* Generate all positive divisors of n.  */
static void generate_divisors(idx_t n, idx_t **out, size_t *len)
{
    /* Factor n.  */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pn = 0;
    factor_u64(n, primes, exps, &pn);

    /* Recursive construction.  */
    idx_t *divs = NULL;
    size_t dlen = 0;
    size_t dcap = 0;
    gen_div_rec((idx_t)1, 0, primes, exps, pn, &divs, &dlen, &dcap);
    *out = divs;
    *len = dlen;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* Undefined for n == 0, 0 is a safe placeholder */

    /* 1. Make sure we have all primes up to n */
    ensure_primes(n);

    /* 2. Pre‑compute exponent of each prime in n! */
    idx_t *fact_exp = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    for (size_t i = 0; i < prime_cnt; ++i)
        fact_exp[i] = exp_in_factorial(n, prime_list[i]);

    /* 3. Generate all divisors of n */
    idx_t *divs;
    size_t dlen;
    generate_divisors(n, &divs, &dlen);

    /* 4. Accumulate the sum */
    seq_t sum = 0;   /* starts at 0 */

    for (size_t di = 0; di < dlen; ++di)
    {
        idx_t d = divs[di];
        idx_t m = n / d;                /* n/d */
        seq_t term = 1;                 /* multiplicative identity */

        /* Loop over all primes to set the exponent of each in the term */
        for (size_t pi = 0; pi < prime_cnt; ++pi)
        {
            uint64_t p   = prime_list[pi];
            idx_t    e    = fact_exp[pi]
                           - exp_in_factorial(d, p)
                           - (idx_t)d * exp_in_number(m, p);

            if (e > 0)
                term = term * big_pow((seq_t)p, (seq_t)e);
        }

        /* Add or subtract the term depending on parity of d */
        if (d & 1)
            sum = sum + term;
        else
            sum = sum - term;
    }

    /* 5. Clean up */
    free(divs);
    free(fact_exp);

    return sum;
}
