/*
 * A169815  a(n) = lcm(1, 2, ..., n)^(n-1)/(n!*(n-1)!).
 * https://oeis.org/A169815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(1, 2, ..., n)^(n-1) / (n! * (n-1)!)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses prime factorisation.  For each prime p ≤ n,
   the exponent of p in the result is
        e(p) = floor(log_p(n))·(n–1) – (exp_p(n!) + exp_p((n–1)!)),
   where exp_p(m!) is the exponent of p in m! (Legendre’s formula). */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/build_primes.h"

/* Return the exponent of prime p in k! */
static idx_t fact_prime_exp(idx_t k, idx_t p)
{
    idx_t exp = 0;
    if (k == 0) return 0;
    for (idx_t div = p; div <= k; div *= p) {
        exp += k / div;
        if (div > k / p) break;         /* prevent overflow of div *= p */
    }
    return exp;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;  /* not used but harmless */

    /* Cache primes up to the largest n seen */
    static idx_t *prime_arr = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    if (n > prime_limit) {
        /* Rebuild primes up to n.  Old memory is leaked on purpose. */
        if (prime_arr) free(prime_arr);
        build_primes(n, &prime_arr, &prime_cnt);
        prime_limit = n;
    }

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_arr[i];
        if (p > n) break;

        /* exponent of p in lcm(1..n): largest e with p^e ≤ n */
        idx_t exp_lcm = 0;
        idx_t power = p;
        while (power <= n) {
            ++exp_lcm;
            if (power > n / p) break;      /* prevent overflow */
            power *= p;
        }

        /* exponents of p in n! and (n-1)! */
        idx_t exp_n_fact   = fact_prime_exp(n,      p);
        idx_t exp_nm1_fact = fact_prime_exp(n-1,    p);

        /* total exponent in the result */
        idx_t total_exp = exp_lcm * (n - 1) - exp_n_fact - exp_nm1_fact;
        if (total_exp == 0) continue;

        /* multiply result by p^total_exp */
        seq_t term = big_pow((seq_t)p, (seq_t)total_exp);
        result *= term;
    }

    return result;
}
