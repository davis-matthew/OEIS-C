/*
 * A227397  Related to Pisano periods: Numbers k such that the period of
 *          Fibonacci numbers mod k equals k+2.
 * https://oeis.org/A227397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 125, u32 n <= 125, u64 n <= 125,
 *            u128 n <= 125, big any n
 * Helpers:   factor_u64, gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate k ≡ 4 (mod 6) and return the n‑th k
   whose Pisano period equals k+2.  The Pisano period of a
   composite modulus is the least common multiple of the
   periods of its prime‑power factors.  For a prime power
   p^e (p≠2,5) the period equals π(p)·p^(e‑1); for p=5 it
   is 20·5^(e‑1); for p=2 it is 3·2^(e‑1) for e≥3 (with
   π(2)=3, π(4)=6).  π(p) is found by testing divisors of
   p-1 or 2(p+1). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

static inline void fib_pair_u64(uint64_t n, uint64_t mod, uint64_t *fn,
                               uint64_t *fn1)
{
    if (n == 0) {
        *fn = 0;
        *fn1 = 1;
        return;
    }
    uint64_t a, b;
    fib_pair_u64(n >> 1, mod, &a, &b);
    /* c = a * (2*b - a) mod mod */
    uint64_t t = ( ( (__int128)2 * b ) % mod + mod - a ) % mod;
    uint64_t c = (uint64_t)((__int128)a * t % mod);
    /* d = a^2 + b^2 mod mod */
    uint64_t d = (uint64_t)(((__int128)a * a + (__int128)b * b) % mod);
    if (n & 1) {
        *fn  = d;
        *fn1 = (c + d) % mod;
    } else {
        *fn  = c;
        *fn1 = d;
    }
}

static inline seq_t pow_uint64(seq_t base, uint64_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* Pisano period of a prime p > 1 */
static seq_t pi_prime(uint64_t p)
{
    if (p == 2) return (seq_t)3;
    if (p == 3) return (seq_t)8;
    if (p == 5) return (seq_t)20;

    uint64_t cand;
    if (p % 10 == 1 || p % 10 == 9)
        cand = p - 1;
    else
        cand = 2 * (p + 1);

    /* reduce cand by prime divisors for which property holds */
    bool reduced = true;
    while (reduced) {
        reduced = false;
        size_t primes_cnt, i;
        uint64_t primes_arr[64], exps_arr[64];
        factor_u64(cand, primes_arr, exps_arr, &primes_cnt);
        for (i = 0; i < primes_cnt; ++i) {
            uint64_t q = primes_arr[i];
            uint64_t m = cand / q;
            uint64_t f0, f1;
            fib_pair_u64(m, p, &f0, &f1);
            if (f0 == 0 && f1 == 1) {
                cand = m;
                reduced = true;
                break;
            }
        }
    }
    return (seq_t)cand;
}

/* Pisano period of an arbitrary positive integer k */
static seq_t pisano_period(seq_t K)
{
    uint64_t k = (uint64_t)K;
    uint64_t primes_arr[64], exps_arr[64];
    size_t primes_cnt;
    factor_u64(k, primes_arr, exps_arr, &primes_cnt);

    seq_t period = 1;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p   = primes_arr[i];
        uint64_t e   = exps_arr[i];
        seq_t pi_pe;
        if (p == 2) {
            /* pi(2^e) = 3·2^(e-1) for e>=3, with pi(2)=3, pi(4)=6 */
            if (e == 1)
                pi_pe = (seq_t)3;
            else if (e == 2)
                pi_pe = (seq_t)6;
            else
                pi_pe = ((seq_t)3) * pow_uint64((seq_t)2, e - 1);
        } else if (p == 5) {
            pi_pe = ((seq_t)20) * pow_uint64((seq_t)5, e - 1);
        } else {
            seq_t pi_p = pi_prime(p);
            pi_pe = pi_p * pow_uint64((seq_t)p, e - 1);
        }
        seq_t g = gcd(period, pi_pe);
        period = period / g * pi_pe;
    }
    return period;
}

/* exported function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset 1; behaviour irrelevant below */

    uint64_t k = 4;   /* first number ≡4 (mod 6) */
    idx_t found = 0;

    while (true) {
        seq_t period = pisano_period((seq_t)k);
        if (period == (seq_t)(k + 2)) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
        k += 6;
    }
}
