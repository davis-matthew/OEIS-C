/*
 * A133881  Even numbers k such that binomial(k+p,k) mod k = 1, where p=10.
 * https://oeis.org/A133881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th even k such that C(k+10,k) mod k = 1, p = 10 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

/* smallest prime factor table */
static idx_t *spf = NULL;
static idx_t  spf_limit = 0;

/* cache of computed terms */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t  last_tested = 2;          /* last even number examined */

/* add prime p with delta exponent into arrays */
static void add_exp(uint32_t *primes, int *exps, size_t *cnt,
                    uint32_t p, int delta)
{
    for (size_t i = 0; i < *cnt; ++i) {
        if (primes[i] == p) {
            exps[i] += delta;
            return;
        }
    }
    primes[*cnt] = p;
    exps[*cnt]   = delta;
    ++(*cnt);
}

/* compute C(k+10,k) modulo k for an even k */
static seq_t binom_mod_k(idx_t k)
{
    /* list of primes and exponents in the factorisation of the binomial */
    uint32_t primes[64];
    int     exps[64];
    size_t  cnt = 0;

    /* denominator 10! = 2^8 · 3^4 · 5^2 · 7^1 */
    static const uint32_t denom_p[4] = {2,3,5,7};
    static const int     denom_e[4] = {8,4,2,1};
    for (int i = 0; i < 4; ++i)
        add_exp(primes, exps, &cnt, denom_p[i], -denom_e[i]);

    /* numerator (k+1)…(k+10) */
    for (idx_t j = 1; j <= 10; ++j) {
        idx_t v = k + j;
        while (v > 1) {
            idx_t p = spf[v];
            int   e = 0;
            while (v % p == 0) {
                v  /= p;
                ++e;
            }
            add_exp(primes, exps, &cnt, (uint32_t)p, e);
        }
    }

    /* combine prime^exp modulo k */
    seq_t r = 1 % (seq_t)k;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t base  = (seq_t)primes[i] % k;
        seq_t pow   = 1 % k;
        int   exp   = exps[i];
        while (exp > 0) {
            if (exp & 1)  pow = (pow * base) % k;
            base = (base * base) % k;
            exp >>= 1;
        }
        r = (r * pow) % k;
    }
    return r;
}

/* ensure enough terms cached to answer n */
static void generate_upto(size_t need)
{
    if (spf == NULL) {
        /* build SPF table with a generous limit (≈20 M) – this is > enough */
        const idx_t LIM = 20000000UL;
        build_spf(LIM, &spf);
        spf_limit = LIM;
    }

    while (cache_len < need) {
        last_tested += 2;                     /* next even number */
        if (last_tested < 4) last_tested = 4; /* start at 4 */

        if (binom_mod_k(last_tested) == 1) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t newcap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) exit(1);           /* allocation failure */
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_len++] = last_tested;
        }
    }
}

/* requested sequence element a(n), n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, harmless fallback */
    generate_upto((size_t)n);
    return cache[(size_t)n - 1];
}
