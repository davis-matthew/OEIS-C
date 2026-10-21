/*
 * A067196  Numbers k such that M(k) = Sum_{i=1..k} mu(phi(i)) where M(k) is
 *          the Mertens function A002321(k).
 * https://oeis.org/A067196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k satisfying M(k) = Σ_{i=1..k} μ(φ(i)),
 *            where M(k) = Σ_{i=1..k} μ(i) (Mertens function)
 * Range:     u8 n <= 26, u16 n <= 585, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

static idx_t *phi = NULL;          /* φ[1..max_limit] */
static int8_t *mu = NULL;          /* μ[1..max_limit] */
static idx_t *terms = NULL;        /* collected k values */
static size_t terms_len = 0;
static size_t terms_cap = 0;
static idx_t max_limit = 0;        /* current maximum precomputed index */
static idx_t current_k = 0;        /* last processed k */
static int64_t Msum = 0;           /* Σ_{i=1..k} μ(i) */
static int64_t Ssum = 0;           /* Σ_{i=1..k} μ(φ(i)) */

/* Rebuild φ[1..limit] and μ[1..limit] using a linear sieve. */
static void rebuild(idx_t limit)
{
    size_t sz = (size_t)limit + 1;

    free(phi);
    free(mu);
    phi = (idx_t *)malloc(sz * sizeof(idx_t));
    mu  = (int8_t *)malloc(sz * sizeof(int8_t));
    if (!phi || !mu)
        abort();

    bool *isComp = (bool *)malloc(sz * sizeof(bool));
    if (!isComp)
        abort();
    for (size_t i = 0; i < sz; ++i)
        isComp[i] = false;

    /* dynamic array of primes */
    idx_t *primes = (idx_t *)NULL;
    size_t primes_cap = 0, primes_len = 0;

    mu[1]  = 1;
    phi[1] = 1;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!isComp[(size_t)i]) {           /* i is prime */
            if (primes_len == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 8;
                idx_t *tmp = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[primes_len++] = i;
            phi[i] = (idx_t)(i - 1);
            mu[i]  = -1;
        }
        for (size_t j = 0; j < primes_len; ++j) {
            idx_t p = primes[j];
            idx_t v = i * p;
            if (v > limit) break;
            isComp[(size_t)v] = true;
            if (i % p == 0) {                /* p divides i */
                phi[v] = phi[i] * (idx_t)p;
                mu[v]  = 0;
                break;
            } else {
                phi[v] = phi[i] * (idx_t)(p - 1);
                mu[v]  = -mu[i];
            }
        }
    }

    free(isComp);
    free(primes);
}

/* Append a new term to the terms array. */
static void push_term(idx_t k)
{
    if (terms_len == terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 8;
        idx_t *tmp = (idx_t *)realloc(terms, terms_cap * sizeof(idx_t));
        if (!tmp) abort();
        terms = tmp;
    }
    terms[terms_len++] = k;
}

/* Ensure that at least n terms are precomputed. */
static void ensure_terms(idx_t n)
{
    while (terms_len < n) {
        idx_t new_limit = (max_limit == 0) ? 1000 : max_limit * 2;
        rebuild(new_limit);

        for (idx_t k = current_k + 1; k <= new_limit; ++k) {
            int8_t mu_k = mu[(size_t)k];
            Msum += (int64_t)mu_k;

            idx_t phi_k = phi[(size_t)k];
            int8_t mu_phi_k = mu[(size_t)phi_k];
            Ssum += (int64_t)mu_phi_k;

            if (Msum == Ssum) {
                push_term(k);
            }
        }
        current_k = new_limit;
        max_limit = new_limit;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined, but safe fallback */
    ensure_terms(n);
    return (seq_t)terms[n - 1];
}
