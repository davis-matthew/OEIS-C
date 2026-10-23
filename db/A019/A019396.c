/*
 * A019396  Primes with primitive root 73.
 * https://oeis.org/A019396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes, factor_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes the sequence of primes for which 73 is a primitive root 
 * (OEIS A019396).  The algorithm searches primes incrementally, testing
 * whether 73 generates the multiplicative group modulo each prime.
 * All tests are performed by modular exponentiation; a prime p is kept
 * if for every distinct prime divisor q of (p‑1) we have
 * 73^((p‑1)/q) ≠ 1 (mod p).
 *
 * The routine uses the helper functions:
 *   - build_primes(idx_t limit, idx_t **out, size_t *cnt)
 *   - mod_pow(seq_t base, uint64_t exp, seq_t mod)
 *   - factor_u64(uint64_t n, uint64_t primes[], uint64_t exps[], size_t *cnt)
 *
 * The function supports any seq_t, idx_t combination supplied by the
 * build system, and is safe for repeated calls with arbitrary n.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/build_primes.h"   /* build_primes */
#include "../utils/mod_pow.h"        /* mod_pow */
#include "../utils/factor_u64.h"     /* factor_u64 */

/* Static state -------------------------------------------------------- */

static bool            initialized  = false;    /* first call performed?      */
static idx_t           sieve_limit  = 0;        /* current upper bound of sieve */
static idx_t          *prime_list   = NULL;     /* primes up to sieve_limit     */
static size_t          prime_cnt    = 0;        /* number of primes                */

static seq_t          *roots        = NULL;     /* primes with primitive root 73  */
static size_t          roots_cnt    = 0;        /* number collected                */
static size_t          roots_cap    = 0;        /* allocated capacity for roots    */

/* Rebuild the prime list and the roots array up to 'limit'           */
static void rebuild(idx_t limit)
{
    /* Free any previous dynamic data --------------------------------- */

    free(prime_list);
    free(roots);

    /* Build all primes ≤ limit -------------------------------------- */

    build_primes(limit, &prime_list, &prime_cnt);

    /* Prepare storage for the roots ---------------------------------- */

    roots_cap = (prime_cnt > 1024) ? prime_cnt : 1024;
    roots = (seq_t *)malloc(roots_cap * sizeof(seq_t));
    roots_cnt = 0;

    /* Examine each prime -------------------------------------------- */

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = (uint64_t)prime_list[i];

        /* Exclude the prime 73 itself ------------------------------- */

        if (p == 73ULL) {
            continue;
        }

        /* p = 2 is always a primitive root (trivial multiplicative group) */
        if (p == 2ULL) {
            if (roots_cnt == roots_cap) {
                roots_cap <<= 1;
                roots = (seq_t *)realloc(roots, roots_cap * sizeof(seq_t));
            }
            roots[roots_cnt++] = (seq_t)p;
            continue;
        }

        /* Factor p-1 to get its distinct prime divisors.               */
        /* 64‑bit numbers have at most 20 distinct prime factors.       */
        uint64_t pr[64], ex[64];
        size_t   fcnt = 0;
        factor_u64(p - 1, pr, ex, &fcnt);

        bool primitive = true;
        for (size_t j = 0; j < fcnt && primitive; ++j) {
            /* Skip duplicate prime factors (if any) -------------------- */
            if (j > 0 && pr[j] == pr[j-1]) {
                continue;
            }

            /* Test Euler criterion for each divisor q ----------------- */
            seq_t r = mod_pow((seq_t)73, (p - 1) / pr[j], (seq_t)p);
            if ((uint64_t)r == 1ULL) {
                primitive = false;
            }
        }

        if (primitive) {
            if (roots_cnt == roots_cap) {
                roots_cap <<= 1;
                roots = (seq_t *)realloc(roots, roots_cap * sizeof(seq_t));
            }
            roots[roots_cnt++] = (seq_t)p;
        }
    }

    /* The prime list can be freed now -------------------------------- */

    free(prime_list);
    prime_list = NULL;
    prime_cnt  = 0;
}

/* Ensure that we have at least 'n' roots in the array                    */
static void ensure_terms(idx_t n)
{
    if (!initialized) {
        /* Start with a reasonable initial bound --------------------- */
        sieve_limit = 200000ULL;
        rebuild(sieve_limit);
        initialized = true;
    }

    /* Extend the sieve until enough primes are collected ---------- */
    while (roots_cnt < n) {
        sieve_limit <<= 1;           /* double the bound                       */
        rebuild(sieve_limit);
    }
}

/* Main entry point ----------------------------------------------------- */

seq_t a(idx_t n)
{
    /* OEIS offset is 1, but we gracefully handle n==0                */
    if (n == 0) {
        return 0;
    }

    ensure_terms(n);
    /* roots[] is 0‑based, a(1) is roots[0]                           */
    return roots[n - 1];
}
