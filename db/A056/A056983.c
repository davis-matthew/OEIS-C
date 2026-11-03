/*
 * A056983  Primes p which divide Sum_{i=1..m} i! for some m (see A125138).
 * https://oeis.org/A056983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A056983 – primes p for which ∑_{i=1}^{m} i! ≡ 0 (mod p) for
    some m.  The value a(n) (n ≥ 1) is the n‑th such prime.  
    The sequence is generated on demand: primes are tested
    sequentially and the qualifying ones are cached.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* minimal grow helper */
#ifdef __cplusplus
template <class T>
static T *grow_ptr(T *ptr, size_t *cap, size_t need, size_t elem_size)
{
    if (*cap < need) {
        size_t new_cap = *cap ? *cap * 2 : 16;
        while (new_cap < need) new_cap *= 2;
        ptr = mem_resize(T, ptr, (new_cap * elem_size) / sizeof(T));
        *cap = new_cap;
    }
    return ptr;
}
#else
static void *grow_ptr(void *ptr, size_t *cap, size_t need, size_t elem_size)
{
    if (*cap < need) {
        size_t new_cap = *cap ? *cap * 2 : 16;
        while (new_cap < need) new_cap *= 2;
        ptr = realloc(ptr, new_cap * elem_size);
        *cap = new_cap;
    }
    return ptr;
}
#endif

/* ----- test whether a prime p divides ∑_{i=1}^{m} i! for some m < p -------- */
static bool prime_divides_factorial_sum(uint64_t p)
{
    uint64_t fact = 1 % p;   /* 1! modulo p */
    uint64_t sum  = 0;
    for (uint64_t i = 1; i < p; ++i) {
        fact = (uint64_t)(((__uint128_t)fact * i) % p);
        sum += fact;
        if (sum >= p) sum -= p;   /* simple mod reduction */
        if (sum == 0) return true;
    }
    return false;
}

/* ----- global storage --------------------------------------------------- */
static seq_t *terms      = NULL;            /* cached sequence values       */
static size_t terms_len  = 0;                /* how many terms stored       */
static size_t terms_cap  = 0;                /* capacity of terms array     */

static uint64_t *primes   = NULL;            /* known primes                */
static size_t primes_len  = 0;                /* count of primes found       */
static size_t primes_cap  = 0;                /* capacity of primes array    */

static uint64_t next_candidate = 2;          /* next number to test for primality */

/* ----- ensure that we have at least n terms in the sequence  ---------------- */
static void ensure_terms(size_t n)
{
    /* grow sequence storage if needed */
    while (terms_len < n) {
        /* find next prime */
        uint64_t p = next_candidate;
        for (;;) {
            bool is_prime = true;
            /* trial division using already found primes */
            for (size_t i = 0; i < primes_len; ++i) {
                uint64_t q = primes[i];
                if ((unsigned __int128)q * q > p) break;
                if (p % q == 0) { is_prime = false; break; }
            }
            if (is_prime) break;           /* p is next prime */
            ++p;
        }

        /* store prime for future divisibility tests */
        primes = (uint64_t *)grow_ptr(primes, &primes_cap, primes_len + 1,
                                      sizeof(uint64_t));
        primes[primes_len++] = p;

        /* test the prime */
        if (p > 2 && prime_divides_factorial_sum(p)) {
            terms = (seq_t *)grow_ptr(terms, &terms_cap, terms_len + 1,
                                      sizeof(seq_t));
            terms[terms_len++] = (seq_t)p;
        }

        /* advance candidate for next search */
        if (p == 2) next_candidate = 3;
        else next_candidate = p + 2;   /* skip even numbers */
    }
}

/* ----- required function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* sequence is 1‑based */
    ensure_terms((size_t)n);
    return terms[(size_t)n - 1];
}
