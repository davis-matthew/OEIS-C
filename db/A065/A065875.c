/*
 * A065875  Numbers k such that usigma(phi(k)) is a prime.
 * https://oeis.org/A065875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   ipow_u64_u32_to_u64, is_prime_u128, is_prime_u64, mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A065875: numbers k such that the unitary sum of divisors of φ(k) is prime */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <stddef.h>
#include <limits.h>
#include "../utils/ipow_u64_u32_to_u64.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/totient.h"
#include "../utils/mem.h"

/* ---------- Trial division factorisation ----------------------------------- */
static void _factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    *cnt = 0;
    if (n < 2) return;
    /* factor 2 separately */
    if ((n & 1ULL) == 0) {
        pr[*cnt] = 2ULL;
        exp[*cnt] = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            exp[*cnt] += 1;
        }
        (*cnt)++;
    }
    /* odd primes */
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0) {
            pr[*cnt] = d;
            exp[*cnt] = 0;
            while (n % d == 0) {
                n /= d;
                exp[*cnt] += 1;
            }
            (*cnt)++;
        }
    }
    if (n > 1) {
        pr[*cnt] = n;
        exp[*cnt] = 1;
        (*cnt)++;
    }
}

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Cache for the sequence values */
static seq_t *terms   = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Next integer to test */
static idx_t next_k = 1;

/* Ensure that the cache can hold at least 'needed' elements. */
static void ensure_terms_cap(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    /* Explicit cast required for C++ compatibility */
    seq_t *new_terms = mem_resize(seq_t, terms, new_cap);
    if (!new_terms) abort();            /* allocation failure */
    terms = new_terms;
    terms_cap = new_cap;
}

/* Test whether k satisfies:  usigma(phi(k)) is prime. */
static bool satisfies_k(idx_t k)
{
    /* Compute φ(k) */
    seq_t phi_seq = totient(k);
    uint64_t phi = (uint64_t)phi_seq;     /* φ(k) ≤ k ≤ 2^64-1 */

    /* Factor φ(k) */
    uint64_t pr[64];
    uint64_t exp[64];
    size_t cnt;
    _factor_u64(phi, pr, exp, &cnt);

    /* Compute the unitary sigma: ∏ (1 + p^e) */
    seq_t u_sigma = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p_pow = ipow_u64_u32_to_u64(pr[i], (unsigned)exp[i]);   /* p^e fits in uint64_t */
        u_sigma *= ((seq_t)p_pow + 1);
    }

    /* Check primality of the result */
    return isPrimeGeneric(u_sigma);
}

/* Main sequence function: a(n) returns the n-th element (n≥1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined for n=0, avoid UB */

    /* Grow the cache until it contains at least n terms. */
    while (terms_len < n)
    {
        if (satisfies_k(next_k))
        {
            ensure_terms_cap(terms_len + 1);
            terms[terms_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return terms[n - 1];
}
