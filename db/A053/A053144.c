/*
 * A053144  Cototient of the n-th primorial number.
 * https://oeis.org/A053144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002110(n) – φ(A002110(n)), * where A002110(n) is the
 *            n-th primorial Qₙ = p₁×p₂×…×pₙ * and φ denotes Euler's totient.
 *            * * Recurrence used: * a(1) = 1 * For n > 1: * let pₙ be the
 *            n‑th prime, * let Qₙ₋₁ be the primorial of the first n–1 primes,
 *            * and let a(n–1) be the previous term. * Then...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 338 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

/* Compute the next prime greater than 'cur' using a simple
 * trial division against the primes already stored. */
static uint64_t next_prime_uint64(uint64_t cur, uint64_t *primes, size_t prime_len)
{
    uint64_t cand = cur + 1;
    for (;; ++cand) {
        bool composite = false;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { composite = true; break; }
        }
        if (!composite) return cand;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Handle the (unrequested) offset n = 0 safely. */
    if (n == 0) return (seq_t)0;

    /* Static storage for memoisation. */
    static seq_t *values      = NULL;   /* values[0] = a(1), etc. */
    static size_t values_cap  = 0;
    static size_t values_len  = 0;

    static uint64_t *primes   = NULL;   /* primes[0] = 2, primes[i] = p_{i+1}. */
    static size_t prime_cap   = 0;
    static size_t prime_len   = 0;

    static seq_t primorial   = 0;       /* Qₙ for the last computed n. */

    /* Initial setup for the first term. */
    if (values_len == 0) {
        /* Allocate space for a(1). */
        ensure_capacity(1, &values_cap, OEISC_VPP(&values), sizeof(seq_t));
        values[0]            = (seq_t)1;          /* a(1) = 1. */
        values_len           = 1;
        primorial           = (seq_t)2;           /* Q₁ = 2. */

        /* Allocate space for the first prime. */
        ensure_capacity(1, &prime_cap, OEISC_VPP(&primes), sizeof(uint64_t));
        primes[0]            = 2;                /* p₁ = 2. */
        prime_len            = 1;
    }

    /* Extend the cache up to the requested index n. */
    while (values_len < n) {
        /* Ensure we have the next prime pₙ. */
        if (prime_len <= values_len) {
            uint64_t p = next_prime_uint64(primes[prime_len - 1], primes, prime_len);
            ensure_capacity(prime_len + 1, &prime_cap, OEISC_VPP(&primes), sizeof(uint64_t));
            primes[prime_len] = p;
            prime_len++;
        }
        uint64_t p = primes[values_len];   /* pₙ, where values_len = n‑1. */

        /* Compute the new term using the recurrence. */
        seq_t a_prev   = values[values_len - 1];
        seq_t new_a    = a_prev * ((seq_t)(p - 1)) + primorial;
        seq_t new_q    = primorial * (seq_t)p;  /* Qₙ  = Qₙ₋₁ · p. */

        ensure_capacity(values_len + 1, &values_cap, OEISC_VPP(&values), sizeof(seq_t));
        values[values_len] = new_a;
        values_len++;

        primorial = new_q;
    }

    return values[n - 1];
}
