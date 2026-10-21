/*
 * A066887  Numbers k such that Omega(phi(k)) = Omega(k) where phi is the
 *          Euler totient function and Omega(k) is the number of repeated
 *          prime factors in k.
 * https://oeis.org/A066887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 1649, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A066887: numbers k such that Omega(phi(k)) = Omega(k).
   Since no closed‑form formula is known, the terms are generated on
   demand by sieving integers and testing the equality of Omega values. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* The integer types seq_t and idx_t are supplied by the build; they are
   not defined here. */

static uint32_t *spf = NULL;          /* smallest prime factor for 1..limit */
static uint8_t  *omega = NULL;        /* Omega(i) for 1..limit */
static size_t   sieve_limit = 0;     /* current maximum sieved value */
static uint32_t last_checked = 1;     /* next integer to test          */

static seq_t   *terms = NULL;        /* array of sequence terms a(1),a(2),… */
static size_t  terms_len = 0;        /* number of terms stored         */
static size_t  terms_cap = 0;        /* capacity of terms array        */

/* Build a linear sieve up to 'limit' and pre‑compute Omega values. */
static void build_sieve(size_t limit)
{
    free(spf);
    free(omega);

    spf   = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    omega = (uint8_t  *)malloc((limit + 1) * sizeof(uint8_t));
    if (!spf || !omega)
        abort();

    uint32_t *primes = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!primes)
        abort();

    size_t primes_sz = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[primes_sz++] = i;
        }
        for (size_t j = 0; j < primes_sz &&
                 primes[j] <= spf[i] &&
                 (uint64_t)i * primes[j] <= limit; ++j)
            spf[i * primes[j]] = primes[j];
    }
    free(primes);

    omega[1] = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        omega[i] = omega[i / spf[i]] + 1;

    sieve_limit = limit;
}

/* Test whether integer k satisfies Omega(phi(k)) == Omega(k). */
static bool test_k(uint32_t k)
{
    if (k == 1)
        return true;   /* Omega(1) = Omega(phi(1)) = 0 */

    uint32_t x = k;
    uint8_t omega_k = 0;    /* Omega(k) */
    uint8_t omega_phi = 0;  /* Omega(phi(k)) */

    while (x > 1) {
        uint32_t p = spf[x];
        uint8_t exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
        }
        omega_k += exp;

        /* Contribution to Omega(phi(k)):
           (exp-1) from p^(exp-1) and Omega(p-1). */
        if (exp > 1)
            omega_phi += (exp - 1);
        omega_phi += omega[p - 1];
    }
    return omega_k == omega_phi;
}

/* Ensure that at least 'need' terms have been generated. */
static void ensure_terms(size_t need)
{
    if (terms_len >= need)
        return;

    if (!spf)
        build_sieve(2000000); /* initial sieve size */

    for (;;) {
        for (; last_checked <= sieve_limit && terms_len < need; ++last_checked) {
            if (test_k(last_checked)) {
                if (terms_len == terms_cap) {
                    size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                    seq_t *new_arr = (seq_t *)realloc(terms,
                                                      new_cap * sizeof(seq_t));
                    if (!new_arr)
                        abort();
                    terms = new_arr;
                    terms_cap = new_cap;
                }
                terms[terms_len++] = (seq_t)last_checked;
            }
        }
        if (terms_len >= need)
            break;

        /* Need a larger sieve. */
        size_t old_limit = sieve_limit;
        size_t new_limit = sieve_limit * 2;
        build_sieve(new_limit);
        last_checked = old_limit + 1;
    }
}

/* Public function to return a(n), n >= 1. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* undefined for n = 0 */
    ensure_terms((size_t)n);
    return terms[(size_t)n - 1];
}
