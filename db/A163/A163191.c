/*
 * A163191  a(n) = Sum_{k=0..n} (-1)^(n-k)*C(n,k)*sigma(n,k) for n>0 with
 *          a(0)=1.
 * https://oeis.org/A163191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (d-1)^n (a(0) = 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of divisors is done via an SPF sieve. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

static idx_t *spf_arr = NULL;
static idx_t spf_limit = 0;

/* Build or extend a smallest‑prime‑factor table up to `limit`. */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit) return;
    free(spf_arr);
    spf_arr = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= limit; ++i) spf_arr[i] = i;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf_arr[i] == i) {
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf_arr[j] == j) spf_arr[j] = i;
            }
        }
    }
    spf_limit = limit;
}

/* Recursive helper to generate all divisors from prime factorisation. */
static void add_divisor_terms(idx_t idx, idx_t cur, idx_t n,
                              seq_t *sum, idx_t primes[], idx_t exps[],
                              idx_t pcnt)
{
    if (idx == pcnt) {
        if (cur > 1) {
            seq_t base = (seq_t)(cur - 1);
            seq_t term = big_pow(base, (seq_t)n);
            *sum += term;
        }
        return;
    }
    idx_t p = primes[idx];
    idx_t emax = exps[idx];
    idx_t cur_mul = 1;
    for (idx_t e = 0; e <= emax; ++e) {
        add_divisor_terms(idx + 1, cur * cur_mul, n, sum,
                          primes, exps, pcnt);
        cur_mul *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    ensure_spf(n);

    /* Factorise n.  The number of distinct prime factors of n ≤ 10 000
       is always ≤ 16, so a small fixed array suffices. */
    idx_t primes[16];
    idx_t exps[16];
    idx_t pcnt = 0;
    idx_t temp = n;
    while (temp > 1) {
        idx_t p = spf_arr[temp];
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        primes[pcnt] = p;
        exps[pcnt] = e;
        ++pcnt;
    }

    seq_t sum = 0;
    add_divisor_terms(0, 1, n, &sum, primes, exps, pcnt);
    return sum;
}
