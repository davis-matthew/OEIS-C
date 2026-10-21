/*
 * A091399  a(n) = Product_{ p | n } (1 + Legendre(7,p) ).
 * https://oeis.org/A091399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the definition by prime factorisation and computing the
   Legendre symbol (7|p) for each distinct prime divisor of n. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---- Sieve of Eratosthenes for smallest prime factors (spf) ---- */
static idx_t *spf = NULL;          /* spf[i] = smallest prime factor of i   */
static idx_t spf_size = 0;         /* maximal n that has been pre‑factored   */

static void init_spf(idx_t limit)
{
    if (limit <= spf_size) return;               /* already big enough */

    /* free any previous array */
    if (spf != NULL) free(spf);

    spf_size = limit;
    spf = (idx_t *)calloc((size_t)(spf_size + 1), sizeof(idx_t));
    if (!spf) {
        abort(); /* allocation failure */
    }

    /* compute spf values */
    for (idx_t i = 2; i <= spf_size; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = i;
            if (i * i <= spf_size) {
                for (idx_t j = i * i; j <= spf_size; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }
}

/* ---- Modular exponentiation (base^exp mod mod) for 64‑bit values ---- */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1) result = (uint64_t)((__uint128_t)result * b % mod);
        b = (uint64_t)((__uint128_t)b * b % mod);
        exp >>= 1;
    }
    return result;
}

/* ---- Legendre symbol (7|p) for an odd prime p ---- */
static int legendre_7(idx_t p)
{
    if (p == 2) return 1;   /* defined as +1 for p = 2   */
    if (p == 7) return 0;   /* symbol is defined as 0     */

    uint64_t exp = (p - 1) / 2;
    uint64_t val = mod_pow_uint64(7, exp, p);
    if (val == 1) return 1;
    if (val == p - 1) return -1;
    return 0;               /* should never happen for prime p */
}

/* ---- Compute a(n) for the OEIS sequence A091399 ---- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1, not used   */
    if (n > spf_size) init_spf(n);        /* prepare spf up to n      */

    seq_t product = 1;
    idx_t temp = n;
    while (temp > 1) {
        idx_t p = spf[temp];               /* distinct prime factor   */
        int leg = legendre_7(p);           /* Legendre symbol (7|p)  */
        seq_t factor = (seq_t)(1 + leg);   /* factor is 0, 1, or 2   */
        product *= factor;

        /* skip all occurrences of p */
        while (temp % p == 0) temp /= p;
    }
    return product;
}
