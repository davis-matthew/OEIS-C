/*
 * A117308  Numbers k for which phi(k)^2 + phi(k) + 1 is a palindrome.
 * https://oeis.org/A117308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th positive integer k such that φ(k)^2 + φ(k) + 1
 *            is a palindrome in base 10. The implementation computes values
 *            on demand, using trial division for φ(k) and a simple dynamic
 *            sieve of primes
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_palindrome.h"

#if defined(__cplusplus)
#include <vector>
#endif

/* ------------------------------------------------------------------ */
/*  Prime list handling                                                */
/* ------------------------------------------------------------------ */
static bool primes_initialized = false;
static idx_t prime_candidate = 3;   /* next odd candidate after 2 */

#if defined(__cplusplus)
static std::vector<idx_t> primes_vec;
static inline size_t primes_size(void) { return primes_vec.size(); }
static inline idx_t  primes_get(size_t i)   { return primes_vec[i]; }
static inline void   primes_append(idx_t p) { primes_vec.push_back(p); }
#else
static idx_t *primes = NULL;
static size_t primes_cap   = 0;
static size_t primes_count = 0;
static inline size_t primes_size(void) { return primes_count; }
static inline idx_t  primes_get(size_t i)   { return primes[i]; }
static inline void   primes_append(idx_t p) {
    if (primes_count == primes_cap) {
        primes_cap = primes_cap ? primes_cap*2 : 16;
        idx_t *tmp = realloc(primes, primes_cap * sizeof(idx_t));
        if (!tmp) exit(1);
        primes = tmp;
    }
    primes[primes_count++] = p;
}
#endif

static void ensure_primes_upto(idx_t limit) {
    if (!primes_initialized) {
        primes_append((idx_t)2);
        prime_candidate = 3;
        primes_initialized = true;
    }
    while (primes_get(primes_size() - 1) < limit) {
        idx_t cand = prime_candidate;
        bool composite = false;
        for (size_t i = 0; i < primes_size(); ++i) {
            idx_t p = primes_get(i);
            if ((idx_t)(p * p) > cand) break;
            if (cand % p == 0) { composite = true; break; }
        }
        if (!composite) primes_append(cand);
        prime_candidate += 2;
    }
}

/* ------------------------------------------------------------------ */
/*  Euler φ computation                                              */
/* ------------------------------------------------------------------ */
static seq_t get_phi(idx_t n) {
    if (n == 1) return (seq_t)1;
    idx_t m = n;
    seq_t phi = (seq_t)n;
    ensure_primes_upto(n);          /* primes up to n are sufficient */
    while (m > 1) {
        bool factored = false;
        for (size_t i = 0; i < primes_size(); ++i) {
            idx_t p = primes_get(i);
            if (p * p > m) break;
            if (m % p == 0) {
                while (m % p == 0) m /= p;
                phi = phi / (seq_t)p * ((seq_t)p - (seq_t)1);
                factored = true;
                break;
            }
        }
        if (!factored) {                 /* remaining m is prime */
            phi = phi / (seq_t)m * ((seq_t)m - (seq_t)1);
            break;
        }
    }
    return phi;
}

/* ------------------------------------------------------------------ */
/*  Sequence storage                                                   */
/* ------------------------------------------------------------------ */
static idx_t lastChecked = 0;   /* largest k examined so far */

#if defined(__cplusplus)
static std::vector<seq_t> terms_vec;
static inline size_t terms_size(void) { return terms_vec.size(); }
static inline seq_t  terms_get(idx_t i)   { return terms_vec[i]; }
static inline void  terms_append(seq_t v) { terms_vec.push_back(v); }
#else
static seq_t *terms = NULL;
static size_t terms_cap   = 0;
static size_t terms_count = 0;
static inline size_t terms_size(void) { return terms_count; }
static inline seq_t  terms_get(idx_t i)   { return terms[i]; }
static inline void  terms_append(seq_t v) {
    if (terms_count == terms_cap) {
        terms_cap = terms_cap ? terms_cap*2 : 16;
        seq_t *tmp = realloc(terms, terms_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        terms = tmp;
    }
    terms[terms_count++] = v;
}
#endif

static void ensure_terms(idx_t n) {
    while (terms_size() < (size_t)n) {
        ++lastChecked;
        seq_t phi_k = get_phi(lastChecked);
        seq_t val = phi_k * phi_k + phi_k + (seq_t)1;
        if (is_palindrome(val, (idx_t)10))
            terms_append((seq_t)lastChecked);
    }
}

/* ------------------------------------------------------------------ */
/*  Public function required by the build                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, return zero as safety */
    ensure_terms(n);
    return terms_get(n - 1);
}
