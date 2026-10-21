/*
 * A240670  Numbers n for which all exponents in the prime power factorization
 *          of (2*n)! are odious (A000069).
 * https://oeis.org/A240670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   popcount_is_odious
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the nth member of OEIS sequence A240670.
 *  The sequence consists of integers n for which every exponent in the
 *  prime‑power factorisation of (2*n)! is odious (has an odd popcount).
 *  There is no closed formula, so the term a(n) is obtained by a
 *  brute‑force search: candidates are tested sequentially, and the
 *  first n that satisfy the condition are cached.  The cache is
 *  persistent across calls, allowing any ordering of queries.  The
 *  code is written to be valid in C11 and also in C++17 (when
 *  seq_t is an arbitrary‑precision integer class).  It uses only
 *  integer arithmetic and standard library facilities. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/popcount_is_odious.h"

/* ---------- Prime sieve with dynamic growth -------------------------------- */
#define MAX_PRIME_ARRAY 200000   /* ample for limits up to ~1e6 */
static uint64_t *prime_list   = NULL;
static size_t    prime_cnt    = 0;
static idx_t     prime_limit  = 0;

static void ensure_primes_upto(idx_t n)
{
    if (n <= prime_limit)
        return;

    /* Allocate a temporary boolean array for the sieve */
    bool *is_prime = (bool *)calloc((size_t)n + 1, 1);
    if (!is_prime) return;   /* allocation failure – unlikely */

    for (idx_t i = 2; i <= n; ++i)
        is_prime[i] = true;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (idx_t x = p * p; x <= n; x += p)
                is_prime[x] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i]) ++cnt;

    /* Allocate (or grow) the prime list */
    uint64_t *tmp = (uint64_t *)realloc(prime_list, cnt * sizeof(uint64_t));
    if (!tmp) {
        free(is_prime);
        return;   /* allocation failure – unlikely */
    }
    prime_list = tmp;
    prime_cnt  = cnt;
    prime_limit = n;

    /* Fill the list */
    size_t idx = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i])
            prime_list[idx++] = i;

    free(is_prime);
}

/* ---------- Check whether all exponents of (2*m)! are odious --------- */
static bool exponents_odious(idx_t m)
{
    idx_t two_m = m + m;  /* 2*m */
    ensure_primes_upto(two_m);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > two_m)
            break;

        idx_t e   = 0;
        idx_t tmp = two_m;
        while (tmp) {
            tmp /= p;
            e += tmp;
        }

        if (!popcount_is_odious(e))
            return false;  /* even popcount → not odious */
    }
    return true;
}

/* ---------- Cache for sequence terms ------------------------------------- */
#define MAX_CACHE_SIZE 200000
static seq_t  cache[MAX_CACHE_SIZE];
static size_t cache_used = 0;
static idx_t  next_candidate = 1;   /* first integer to test */

/* ---------- Public function required by the build system ---------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined; sequence offset is 1 */

    while (cache_used < (size_t)n) {
        if (next_candidate > 100000000)   /* safety guard */
            return 0;

        if (exponents_odious(next_candidate))
            cache[cache_used++] = (seq_t)next_candidate;

        ++next_candidate;
    }
    return cache[n - 1];
}
