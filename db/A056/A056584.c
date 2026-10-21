/*
 * A056584  Solution to (n^2/a(n))^a(n) = gcd(n^n, Product_{k<n} k^k) where
 *          a(n) and n^2/a(n) are integers, or 0 if no such integers exist.
 * https://oeis.org/A056584
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 / a , where (n^2 / a)^a = gcd(n^n, ∏_{k<n} k^k), and
 *            a and n^2 / a are integers; otherwise 0
 * Range:     u8 n <= 16, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   sum_k_vp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/sum_k_vp.h"

/*------------------- helper: factor n -------------------*/
static void factorize(idx_t n, idx_t *primes, idx_t *exps, size_t *cnt)
{
    *cnt = 0;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            idx_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            primes[(*cnt)] = p;
            exps[(*cnt)]   = e;
            ++(*cnt);
        }
    }
    if (n > 1) {
        primes[(*cnt)] = n;
        exps[(*cnt)]   = 1;
        ++(*cnt);
    }
}

/*------------------- main sequence function -------------------*/
seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* factorization of n */
    idx_t primes[20], exps[20];
    size_t pcnt;
    factorize(n, primes, exps, &pcnt);

    /* exponents of n^2 (used for divisor generation and checks) */
    idx_t sqexps[20];
    for (size_t i = 0; i < pcnt; ++i) sqexps[i] = 2 * exps[i];

    /* compute gexp[p] = exponent of p in gcd */
    seq_t gexp[20];
    for (size_t i = 0; i < pcnt; ++i) {
        seq_t Sp = sum_k_vp(n, primes[i]);
        seq_t v  = (seq_t)n * (seq_t)exps[i];
        gexp[i]  = (Sp < v) ? Sp : v;
    }

    /* generate all divisors of n^2 */
    /* array large enough for all divisors (max < 2000 for n<=10000) */
    seq_t divs[2000];
    size_t divcnt = 1;
    divs[0] = 1;

    for (size_t pi = 0; pi < pcnt; ++pi) {
        idx_t p      = primes[pi];
        idx_t emax   = sqexps[pi];
        size_t oldcnt = divcnt;
        for (size_t i = 0; i < oldcnt; ++i) {
            seq_t base = divs[i];
            seq_t mult = 1;
            for (idx_t e = 1; e <= emax; ++e) {
                mult *= (seq_t)p;
                divs[divcnt++] = base * mult;
            }
        }
    }

    /* test each candidate divisor a */
    for (size_t ai = 0; ai < divcnt; ++ai) {
        seq_t aval = divs[ai];
        bool ok = true;
        for (size_t pi = 0; pi < pcnt && ok; ++pi) {
            idx_t p = primes[pi];

            /* exponent of p in a */
            idx_t ea = 0;
            seq_t tmp = aval;
            while (tmp % (seq_t)p == 0) {
                tmp /= (seq_t)p;
                ++ea;
            }

            /* exponent of p in d = n^2 / a */
            idx_t exp_d = sqexps[pi] - ea;      /* 2*e_i - ea   */
            seq_t left = aval * (seq_t)exp_d;   /* a * exp_d    */
            if (left != gexp[pi]) ok = false;
        }
        if (ok) return aval;
    }
    return (seq_t)0;
}
