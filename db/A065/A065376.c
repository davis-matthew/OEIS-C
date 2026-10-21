/*
 * A065376  Primes of the form p + k^2, p prime and k > 0.
 * https://oeis.org/A065376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A065376: primes that can be written as p = q + k²
   where q is prime and k > 0.  The code generates the list of
   such primes up to a fixed bound and indexes into it. */
/* Implementation may be compiled as C or C++ (seq_t and idx_t are
   defined by the build system). */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#include <stddef.h>
#endif
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------
   Storage for the computed terms.
   C++ path: std::vector<seq_t>
   C    path: dynamic array with manual capacity handling.
   -------------------------------------------------------------------- */
#ifdef __cplusplus
static std::vector<seq_t> terms;
#else
static seq_t   *terms = NULL;
static idx_t   terms_len = 0;
static idx_t   terms_cap = 0;
#endif

/* --------------------------------------------------------------------
   Helper to append a term.  Keeps sizes consistent for both paths.
   -------------------------------------------------------------------- */
static inline void add_term(seq_t v)
{
#ifdef __cplusplus
    terms.push_back(v);
#else
    if (terms_len == terms_cap) {
        idx_t new_cap = terms_cap ? (terms_cap << 1) : 256;
        seq_t *tmp = (seq_t *)realloc(terms, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out‑of‑memory: terminate */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = v;
#endif
}

/* --------------------------------------------------------------------
   Pre‑compute primes and collect those that satisfy the condition.
   The upper bound is chosen generously enough to contain >10 000 terms.
   -------------------------------------------------------------------- */
static bool terms_initialized = false;

static void precompute_terms(void)
{
    const size_t LIMIT = 500000;            /* sufficient for many terms */

    /* Simple prime sieve (Eratosthenes) */
    char *is_prime = (char *)calloc(LIMIT + 1, sizeof(char));
    if (!is_prime) abort();                 /* memory allocation failed */
    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* Scan primes and test the representation p = q + k² */
    for (size_t p = 3; p <= LIMIT; ++p) {
        if (!is_prime[p]) continue;
        bool ok = false;
        for (size_t k = 1; k * k < p; ++k) {
            size_t rem = p - k * k;
            if (rem >= 2 && is_prime[rem]) {   /* q = rem prime */
                ok = true;
                break;
            }
        }
        if (ok) add_term((seq_t)p);
    }

    free(is_prime);
    terms_initialized = true;
}

/* --------------------------------------------------------------------
   Macro helpers for accessing length and elements regardless of
   compilation mode.
   -------------------------------------------------------------------- */
#ifdef __cplusplus
#define TERMS_LEN  static_cast<size_t>(terms.size())
#define TERMS_ELEM(idx) terms[(size_t)(idx)]
#else
#define TERMS_LEN  terms_len
#define TERMS_ELEM(idx) terms[(idx)]
#endif

/* --------------------------------------------------------------------
   Sequence accessor.
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!terms_initialized) precompute_terms();

    if (n == 0 || n > TERMS_LEN) return (seq_t)0;   /* undefined indices */
    return TERMS_ELEM(n - 1);
}
