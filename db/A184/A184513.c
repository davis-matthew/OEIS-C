/*
 * A184513  L.g.f.: Sum_{n>=1} (x^n/n)/sqrt(1 - 2*(2*x)^n) = Sum_{n>=1}
 *          a(n)*x^n/n.
 * https://oeis.org/A184513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 2^((d-1)*(n/d-1)) * C(2*(d-1),(d-1)) * d
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/binom.h"    /* seq_t binom(idx_t a, idx_t b); */

/* Recursive divisor enumeration */
static void dfs_divisors(idx_t *primes, int *exps, size_t idx, size_t pc,
                         idx_t cur, idx_t n, seq_t *sum)
{
    if (idx == pc) {
        idx_t d   = cur;
        idx_t p1  = d - 1;
        idx_t p2  = n / d - 1;
        seq_t exp = (seq_t)p1 * (seq_t)p2;              /* exponent of 2   */
        seq_t pow2 = big_pow((seq_t)2, exp);            /* 2^exp           */
        seq_t binom_val = binom(2 * p1, p1);            /* central binomial */
        seq_t term = pow2 * binom_val;
        term = term * (seq_t)d;
        *sum += term;
        return;
    }

    idx_t p     = primes[idx];
    idx_t cur2  = cur;
    for (int e = 0; e <= exps[idx]; ++e) {
        dfs_divisors(primes, exps, idx + 1, pc, cur2, n, sum);
        cur2 *= p;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, guard */

    /* Factor n */
    idx_t  tmp   = n;
    idx_t  primes[64];
    int    exps  [64];
    size_t pc    = 0;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pc] = p;
            exps[pc]   = e;
            ++pc;
        }
    }
    if (tmp > 1) {
        primes[pc] = tmp;
        exps[pc]   = 1;
        ++pc;
    }

    seq_t sum = 0;
    dfs_divisors(primes, exps, 0, pc, 1, n, &sum);
    return sum;
}
