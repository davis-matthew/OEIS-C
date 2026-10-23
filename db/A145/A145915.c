/*
 * A145915  Even composites in A145832.
 * https://oeis.org/A145915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   floor_sqrt, seq_alloc_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Correlation function for OEIS sequence A145915:
   a(n) = the nth even composite integer n such that for every divisor 
   d of n, the number k = d + n/d has largest prime divisor ≤ sqrt(k).
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/floor_sqrt.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_realloc.h"

/* --------- Prime sieve for fast factorisation --------- */

#define INITIAL_SIEVE_LIMIT 3000000  /* Sufficient for first 10000 terms */

static uint32_t *spf = NULL;        /* smallest prime factor */
static uint32_t *lpf = NULL;        /* largest prime factor */
static size_t   sieve_limit = 0;

/* Build sieve up to new_limit (always increasing) */
static void build_sieve(size_t new_limit)
{
    if (new_limit <= sieve_limit) return;

    free(spf);
    free(lpf);
    spf = (uint32_t*)malloc((new_limit + 1) * sizeof(uint32_t));
    lpf = (uint32_t*)malloc((new_limit + 1) * sizeof(uint32_t));

    for (size_t i = 0; i <= new_limit; ++i) {
        spf[i] = 0;
        lpf[i] = 0;
    }

    for (size_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {               /* i is prime */
            spf[i] = (uint32_t)i;
            for (size_t j = i; j <= new_limit; j += i) {
                if (spf[j] == 0) spf[j] = (uint32_t)i;
                lpf[j] = (uint32_t)i;     /* last prime divisor will be largest */
            }
        }
    }
    sieve_limit = new_limit;
}

/* --------- Divisor enumeration and property test --------- */

/* Recursively check all divisors of n (prime factorised in primes/exps) */
static bool check_divisors_rec(idx_t nVal, uint32_t *primes, int *exps, int pCnt,
                               int idx, uint64_t cur)
{
    if (idx == pCnt) {
        uint64_t d = cur;
        uint64_t k = d + nVal / d;
        uint32_t lp = lpf[(size_t)k];
        seq_t sq = floor_sqrt(k);
        if (lp > (uint32_t)sq) return false;
        return true;
    }

    for (int e = 0; e <= exps[idx]; ++e) {
        if (!check_divisors_rec(nVal, primes, exps, pCnt, idx + 1, cur))
            return false;
        cur *= primes[idx];
    }
    return true;
}

/* Test whether candidate satisfies the defining property */
static bool satisfies(idx_t candidate)
{
    /* factorise candidate using spf */
    uint32_t primes[32];
    int exps [32];
    int pCnt = 0;

    idx_t temp = candidate;
    while (temp > 1) {
        uint32_t p = spf[(size_t)temp];
        int e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        primes[pCnt] = p;
        exps[pCnt] = e;
        ++pCnt;
    }

    return check_divisors_rec(candidate, primes, exps, pCnt, 0, 1);
}

/* --------- Memoised sequence generation --------- */

static seq_t *terms = NULL;
static size_t terms_cap = 64;
static size_t terms_len = 0;
static idx_t next_candidate = 2;          /* global candidate pointer */

/* Return the nth term (1‑based) of A145915 */
seq_t a(idx_t n)
{
    if (terms == NULL) {                         /* initialise term storage */
        terms = seq_alloc_array(terms_cap);
    }

    /* ensure sieve built once at start */
    if (sieve_limit == 0) {
        build_sieve(INITIAL_SIEVE_LIMIT);
    }

    while (terms_len < n) {
        /* skip values that are too large for the current sieve */
        if ((size_t)next_candidate > sieve_limit) {
            /* enlarge sieve if needed (unlikely with chosen limit) */
            size_t new_limit = sieve_limit * 2;
            if (new_limit < (size_t)next_candidate) new_limit = (size_t)next_candidate;
            build_sieve(new_limit);
        }

        /* even and composite? continue only if that is true */
        if ((next_candidate & 1) == 0
            && spf[(size_t)next_candidate] != (uint32_t)next_candidate)
        {
            if (satisfies(next_candidate)) {          /* property holds */
                if (terms_len == terms_cap) {         /* grow array if needed */
                    size_t old_cap = terms_cap;
                    terms_cap <<= 1;
                    terms = seq_realloc(terms, old_cap, terms_cap);
                }
                terms[terms_len++] = (seq_t)next_candidate;
            }
        }
        ++next_candidate;
    }

    return terms[n - 1];
}
