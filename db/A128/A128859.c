/*
 * A128859  Let p be the n-th odd prime; a(n) is the number of primitive roots
 *          of p which are <= (p-1)/2.
 * https://oeis.org/A128859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = # of primitive roots g of the n-th odd prime p * such
 *            that g ≤ (p-1)/2. * Computation: get the n‑th odd prime via
 *            get_prime(n+1), find a * primitive root g, then count all powers
 *            g^k with 1≤k<p‑1, gcd(k,p‑1)=1, * that fall in [1,(p‑1)/2]
 * Range:     u8 n <= 53, u16 n <= 6829, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorize, gcd, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/get_prime.h"    /* get_prime(idx_t k) returns nth prime */
#include "../utils/mod_pow.h"      /* mod_pow(seq_t base, uint64_t exp, seq_t mod) */
#include "../utils/factorize.h"    /* factorize(uint64_t n, uint64_t primes[], int exps[]) */
#include "../utils/gcd.h"          /* gcd(seq_t a, seq_t b) */

static void factor_phi(uint32_t phi, uint32_t *factors, int *cnt)
{
    uint64_t primes[64];
    int      exps[64];
    int      res = factorize((uint64_t)phi, primes, exps);
    int  nf = 0;
    for (int i = 0; i < res; ++i) {
        if (i == 0 || primes[i] != primes[i-1]) {
            factors[nf++] = (uint32_t)primes[i];
        }
    }
    *cnt = nf;
}

static seq_t compute_value(idx_t n)
{
    /* n ≥ 1.  The n-th odd prime is (n+1)-th prime, since 2 is skipped. */
    uint32_t p = get_prime(n + 1);     /* e.g. n=1 → p=3 */
    uint32_t phi = p - 1;
    uint32_t half = phi / 2;

    /* Factor φ(p–1) to get its distinct prime factors. */
    uint32_t primes_fac[32];
    int       fac_cnt = 0;
    factor_phi(phi, primes_fac, &fac_cnt);

    /* Find any primitive root g of p. */
    uint32_t g = 0;
    for (uint32_t cand = 2; cand < p; ++cand) {
        bool ok = true;
        for (int i = 0; i < fac_cnt; ++i) {
            if (mod_pow((seq_t)cand, (uint64_t)(phi / primes_fac[i]), (seq_t)p) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) {
            g = cand;
            break;
        }
    }
    if (g == 0) return 0; /* safety, should not happen for prime p */

    /* Count primitive roots ≤ half. */
    uint64_t count = 0;
    uint64_t val   = 1;  /* current g^k mod p */
    for (uint32_t k = 1; k < phi; ++k) {
        val = (val * g) % p;
        if (gcd((seq_t)k, (seq_t)phi) == 1 && val <= half) {
            ++count;
        }
    }
    return (seq_t)count;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, safe return */
    return compute_value(n);
}
