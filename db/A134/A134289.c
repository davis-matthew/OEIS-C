/*
 * A134289  Eighth column (and diagonal) of Narayana triangle A001263.
 * https://oeis.org/A134289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+8,8) * binomial(n+8,7) / (n+8)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 14, u64 n <= 88, u128 n <= 1000,
 *            big any n
 * Helpers:   exp_in_t_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/exp_in_t_prime.h"

/* Pre‑compute all primes up to a fixed limit (enough for n up to about 10000). */
#define MAX_LIMIT 20000
static idx_t primes[MAX_LIMIT / 2 + 10];
static size_t primes_cnt = 0;

/* Initialise the prime table once. */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    static bool comp[MAX_LIMIT + 1];
    for (idx_t i = 2; i <= MAX_LIMIT; ++i) {
        if (!comp[i]) {
            primes[primes_cnt++] = i;
            if (i * i <= MAX_LIMIT) {
                for (idx_t j = i * i; j <= MAX_LIMIT; j += i)
                    comp[j] = true;
            }
        }
    }
    done = true;
}

/* Count the exponent of prime p in n! (Legendre). */
static inline idx_t exp_in_fact(idx_t n, idx_t p)
{
    idx_t e = 0;
    for (idx_t div = p; div <= n; div *= p)
        e += n / div;
    return e;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_primes();

    idx_t t = n + 8;
    seq_t result = 1;

    for (size_t i = 0; i < primes_cnt && primes[i] <= t; ++i) {
        idx_t p = primes[i];

        /* exponent of p in a(n) = 2*exp_fact(t,p) - exp_fact(8,p) - exp_fact(t-8,p)
                              - exp_fact(7,p) - exp_fact(t-7,p) - exp_in_t_prime(t,p) */
        idx_t e = 2 * exp_in_fact(t, p)
                  - exp_in_fact(8, p)
                  - exp_in_fact(t - 8, p)
                  - exp_in_fact(7, p)
                  - exp_in_fact(t - 7, p)
                  - exp_in_t_prime(t, p);

        for (idx_t k = 0; k < e; ++k)
            result *= (seq_t)p;
    }

    return result;
}
