/*
 * A074919  Number of integers in {1, 2, ..., phi(n)} that are coprime to n.
 * https://oeis.org/A074919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * floor(phi(n)/d)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   calc_phi, factor_u64, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where phi is Euler's totient function and mu is the Möbius function. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/calc_phi.h"
#include "../utils/mu_of.h"
#include "../utils/factor_u64.h"

/* Recursive helper to generate all divisors of n from its prime factorisation. */
static void gen_divisors(const uint64_t *primes, const uint64_t *exps,
                         size_t pcnt, size_t idx, idx_t cur,
                         idx_t *divs, size_t *divcnt)
{
    if (idx == pcnt) {
        divs[*divcnt] = cur;
        (*divcnt)++;
        return;
    }
    idx_t val = cur;
    for (size_t i = 0; i <= exps[idx]; ++i) {
        gen_divisors(primes, exps, pcnt, idx + 1, val, divs, divcnt);
        val *= (idx_t)primes[idx];
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Euler's totient of n. */
    seq_t phi = calc_phi(n);

    /* 1. factorise n and generate its divisors. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(n, primes, exps, &pcnt);

    /* 2. generate divisors.  For n <= 10^4 the number of divisors is tiny,
       so a stack array is more than enough. */
    idx_t divs[256];
    size_t divcnt = 0;
    gen_divisors(primes, exps, pcnt, 0, 1, divs, &divcnt);

    /* 3. compute the divisor sum using the Möbius function. */
    long long sum = 0;  /* temporary signed accumulator */
    for (size_t i = 0; i < divcnt; ++i) {
        idx_t d = divs[i];
        int mu = mu_of(d);
        if (mu == 0) continue;
        seq_t term = phi / (seq_t)d;      /* floor division */
        sum += mu * (long long)term;
    }

    /* The result is non‑negative; cast to the required type. */
    return (seq_t)sum;
}
