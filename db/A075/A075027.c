/*
 * A075027  Numbers k such that d(k-1) < d(k) > d(k+1), d = A000005.
 * https://oeis.org/A075027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 3403, u32 n <= 3403, u64 n <= 3403,
 *            u128 n <= 3403, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A075027: Numbers k such that d(k-1) < d(k) > d(k+1), d = A000005 */
/* The function returns the nth term of this sequence (offset 1). */

/* ---- Standard headers ---- */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stddef.h>

/* ---- Utility headers (provided by the build system) ---- */
#include "../utils/build_primes_upto.h"
#include "../utils/_floor_isqrt_u64.h"

/* ---- Forward declaration of helper for divisor count ---- */
static seq_t tau_of_n(idx_t n);

/* ---- Static storage for the sequence ---- */
static seq_t *terms = NULL;      /* cached terms   */
static size_t terms_len = 0;     /* number of cached terms */
static size_t terms_cap = 0;     /* capacity of terms array */

/* ---- Static storage for divisor counts (τ values) ---- */
static seq_t *tau = NULL;       /* τ values for 0..max_checked+1 */
static size_t tau_cap = 0;      /* capacity of tau array */

/* ---- Static storage for primes ---- */
static uint64_t *primes = NULL;   /* array of primes up to prime_limit */
static size_t    primes_cnt = 0;   /* number of stored primes */
static idx_t     prime_limit = 0;  /* current maximum prime value */

/* ---- Other static state ---- */
static idx_t max_checked = 1;     /* largest k that has been examined */

/* ---- Helper: append a value to the cached terms ---- */
static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        terms_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
        seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        assert(tmp);
        terms = tmp;
    }
    terms[terms_len++] = val;
}

/* ---- Helper: ensure tau array large enough for index i ---- */
static void ensure_tau_cap(idx_t i)
{
    if ((size_t)i < tau_cap) return;
    size_t new_cap = tau_cap ? tau_cap * 2 : 64;
    while (new_cap <= (size_t)i) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(tau, new_cap * sizeof(seq_t));
    assert(tmp);
    tau = tmp;
    tau_cap = new_cap;
}

/* ---- Helper: ensure primes array contains all primes up to limit ---- */
static void ensure_primes(idx_t limit)
{
    if (prime_limit >= limit) return;
    free(primes);
    build_primes_upto(limit, &primes, &primes_cnt);
    prime_limit = limit;
}

/* ---- Compute τ(n) using the stored primes ---- */
static seq_t tau_of_n(idx_t n)
{
    if (n <= 1) return (seq_t)n;          /* τ(0)=0, τ(1)=1 */
    idx_t m = n;
    seq_t res = 1;
    idx_t sqrt_m = _floor_isqrt_u64(m);
    ensure_primes(sqrt_m);
    for (size_t i = 0; i < primes_cnt && primes[i] <= sqrt_m; ++i) {
        uint64_t p = primes[i];
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            res *= (seq_t)(exp + 1);
            sqrt_m = _floor_isqrt_u64(m);
        }
    }
    if (m > 1) res *= 2;  /* remaining prime factor */
    return res;
}

/* ---- Main sequence function ---- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* below offset, but harmless */

    /* One-time static initialization */
    static bool initialized = false;
    if (!initialized) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        assert(terms);
        tau_cap = 8;
        tau = (seq_t *)malloc(tau_cap * sizeof(seq_t));
        assert(tau);
        tau[0] = 0;
        tau[1] = 1;
        max_checked = 1;
        initialized = true;
    }

    /* Generate terms until we have at least n of them */
    while (terms_len < n) {
        idx_t k = max_checked + 1;          /* next candidate to test */
        /* Ensure tau array can hold values up to k+1 */
        ensure_tau_cap(k + 1);

        /* Compute τ(k) and τ(k+1) */
        tau[k]     = tau_of_n(k);
        tau[k + 1] = tau_of_n(k + 1);

        /* Check for strict local maximum */
        if (tau[k] > tau[k - 1] && tau[k] > tau[k + 1]) {
            append_term((seq_t)k);
        }

        max_checked = k;
    }

    return terms[n - 1];
}
