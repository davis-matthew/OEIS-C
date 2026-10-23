/*
 * A138309  a(1)=1. a(n) = smallest integer > a(n-1) that is coprime to every
 *          (nonzero) exponent in the prime factorization of n.
 * https://oeis.org/A138309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 214, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A138309: a(1)=1, and for n>=2
   a(n) = the smallest integer > a(n-1) which is coprime
   to every exponent appearing in the prime factorisation of n. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t *a_cache = NULL;     /* 1‑based cache of a(n) values   */
static idx_t  a_cache_cap = 0;    /* current allocated size (>= n+1)*/
static idx_t  a_cache_nmax = 0;   /* largest n that has been computed */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= a_cache_cap)
        return;
    size_t new_cap = (size_t)(n + 1);
    seq_t *ptr;
    if (a_cache == NULL)
        ptr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    else
        ptr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (ptr == NULL)
        abort();
    a_cache = ptr;
    a_cache_cap = new_cap;
}

/* Collect all exponents > 1 in the prime factorisation of n.
   exps[0..cnt-1] will contain those exponents as seq_t values. */
static void factor_exponents(idx_t n, seq_t exps[], size_t *cnt_out)
{
    size_t cnt = 0;
    uint64_t m = (uint64_t)n;
    for (idx_t p = 2; (uint64_t)p * (uint64_t)p <= m; ++p) {
        if (m % p != 0)
            continue;
        size_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        if (exp > 1) {
            if (cnt < 64)  /* 64 slots are more than enough for any n <= 2^64 */
                exps[cnt++] = (seq_t)exp;
        }
    }
    /* any remaining factor is a prime > sqrt(n) with exponent 1 – ignore */
    *cnt_out = cnt;
}

/* Main recurrence function for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n=0 */

    ensure_cache(n);

    /* Initialise the first term if not yet set. */
    if (a_cache[1] == 0) {
        a_cache[1] = (seq_t)1;
        a_cache_nmax = 1;
    }

    /* Compute any missing values up to n. */
    for (idx_t i = a_cache_nmax + 1; i <= n; ++i) {
        seq_t exps[64];
        size_t expcnt = 0;
        factor_exponents(i, exps, &expcnt);

        seq_t candidate = a_cache[i - 1] + (seq_t)1;

        if (expcnt == 0) {
            /* No exponent restriction – simply increment. */
            a_cache[i] = candidate;
            continue;
        }

        /* Find smallest integer > a(i‑1) coprime to all stored exponents. */
        while (true) {
            bool ok = true;
            for (size_t k = 0; k < expcnt; ++k) {
                if (gcd(candidate, exps[k]) != 1) {
                    ok = false;
                    break;
                }
            }
            if (ok)
                break;
            ++candidate;
        }
        a_cache[i] = candidate;
    }

    a_cache_nmax = n;
    return a_cache[n];
}
