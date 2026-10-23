/*
 * A128250  LCG periods: periods of the output sequences produced by
 *          multiplicative linear congruential generators (LCGs) with prime
 *          moduli, for all valid combinations of multiplier and modulus.
 * https://oeis.org/A128250
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = multiplicative order of a modulo prime m, where (m,a) are
 *            enumerated by prime m (increasing) and multiplier a (1 … m‑1)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   factorize, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/factorize.h"
#include "../utils/mod_pow.h"

static idx_t primes[1024];
static idx_t cum_terms[1024];
static size_t prime_cnt = 0;
static idx_t last_prime = 0;

static void add_next_prime(void)
{
    idx_t cand = last_prime + 1;
    while (!is_prime(cand))
        cand++;
    primes[prime_cnt] = cand;
    if (prime_cnt == 0)
        cum_terms[prime_cnt] = cand - 1;
    else
        cum_terms[prime_cnt] = cum_terms[prime_cnt - 1] + cand - 1;
    last_prime = cand;
    prime_cnt++;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    if (prime_cnt == 0)
        add_next_prime();                  // ensure prime 2 is present

    idx_t target = n - 2;                 // zero‑based index among all terms
    size_t idx = 0;

    while (true) {
        if (idx >= prime_cnt)
            add_next_prime();

        idx_t p = primes[idx];
        if (target < p - 1)
            break;
        target -= p - 1;
        idx++;
    }

    idx_t a_val = target + 1;             // multiplier
    idx_t m = primes[idx];                // modulus (prime)

    /* multiplicative order of a_val modulo m */
    uint64_t order = m - 1;
    uint64_t fac[16];
    int exp[16];
    size_t fcnt = factorize(m - 1, fac, exp);

    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t q = fac[i];
        while (order % q == 0) {
            uint64_t test = order / q;
            if (mod_pow((seq_t)a_val, test, (seq_t)m) == (seq_t)1)
                order = test;
            else
                break;
        }
    }

    return (seq_t)order;
}
