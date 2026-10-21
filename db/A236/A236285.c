/*
 * A236285  a(n) = tau(n)^sigma(n), where tau(n) = A000005(n) = the number of
 *          divisors of n and sigma(n) = A000203(n) = the sum of divisors of
 *          n.
 * https://oeis.org/A236285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n)^sigma(n) (tau = number of divisors, sigma = sum of
 *            divisors)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_pow.h"
#include "../utils/big_pow.h"

static bool primes_inited = false;
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;

/* Build a small sieve once (covers sqrt(10000) = 100). */
static void init_primes(void) {
    if (!primes_inited) {
        uint64_t *p = NULL;
        size_t cnt = 0;
        build_primes_upto(100ULL, &p, &cnt);
        primes = p;
        prime_cnt = cnt;
        primes_inited = true;
    }
}

/* Compute tau(n) and sigma(n) for an unsigned 64‑bit integer n. */
static void tau_sigma(uint64_t n, seq_t *tau_out, seq_t *sigma_out) {
    seq_t tau = (seq_t)1;
    seq_t sigma = (seq_t)1;
    uint64_t tmp = n;

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            tau *= (seq_t)(exp + 1);

            /* sigma term = (p^(exp+1)-1)/(p-1) */
            seq_t p_pow = seq_pow((seq_t)p, (idx_t)(exp + 1));
            seq_t term = (p_pow - (seq_t)1) / ((seq_t)p - (seq_t)1);
            sigma *= term;
        }
    }

    /* Remaining prime factor, if any. */
    if (tmp > 1) {
        seq_t r = (seq_t)tmp;
        tau *= (seq_t)2;
        sigma *= (r + (seq_t)1);
    }

    *tau_out = tau;
    *sigma_out = sigma;
}

seq_t a(idx_t n) {
    if (n < (idx_t)1)
        return (seq_t)0;   /* undefined for n<1, but safe fallback */

    init_primes();

    seq_t tau, sigma;
    tau_sigma((uint64_t)n, &tau, &sigma);
    return big_pow(tau, sigma);
}
