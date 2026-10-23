/*
 * A176235  Numbers n such that one of nonprime(n)-1 or nonprime(n)+1 is
 *          prime.
 * https://oeis.org/A176235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer m >= 1 such that exactly one of
 *            nonprime(m)-1 and nonprime(m)+1 is prime. nonprime(m) is the
 *            OEIS A018252 sequence (0,1,4,6,8,9,10,12,14,15,16,…). The
 *            implementation builds the nonprime sequence incrementally and
 *            caches the leading terms of A176235. It handles calls in any...
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#if defined(__cplusplus)
#include <vector>
#include <cstddef>
#else
#include <stdlib.h>
#endif
#include "../utils/is_prime.h"

#if defined(__cplusplus)
/* C++ implementation – use std::vector for automatic memory handling. */
static std::vector<seq_t> terms;          /* cache of A176235 terms   */
static std::vector<seq_t> nonprimes;     /* nonprime sequence, index 0 -> 0 */
static idx_t            lastChecked = 0; /* last integer examined in nonprime generation */
static idx_t            currentCandidate = 1; /* next integer to test for the property */

/* Ensure nonprimes vector contains indices up to 'need' (i.e., size > need). */
static void generate_nonprimes_upto(idx_t need)
{
    while ((idx_t)nonprimes.size() <= need) {
        ++lastChecked;
        if (!is_prime(lastChecked))
            nonprimes.push_back((seq_t)lastChecked);
    }
}

/* Ensure that nonprimes[0] == 0 is present. */
static void init_nonprimes(void)
{
    if (nonprimes.empty()) {
        nonprimes.push_back((seq_t)0);
        lastChecked = 0;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* outside defined offset */
    init_nonprimes();                /* make sure 0 is present */
    while (terms.size() < (size_t)n) {
        idx_t cand = currentCandidate;
        generate_nonprimes_upto(cand);
        seq_t np = nonprimes[(size_t)cand];
        bool p1 = is_prime((idx_t)np - 1);
        bool p2 = is_prime((idx_t)np + 1);
        if (p1 ^ p2)
            terms.push_back((seq_t)cand);
        ++currentCandidate;
    }
    return terms[(size_t)(n - 1)];
}

#else
/* C implementation – manual dynamic arrays with malloc/realloc. */
static seq_t *terms = NULL;
static size_t termsCap   = 0;
static size_t termsSize  = 0;

static seq_t *nonprimes = NULL;
static size_t nonprimeCap = 0;
static size_t nonprimeCnt = 0;

static idx_t lastChecked = 0;
static idx_t currentCandidate = 1;

/* Ensure terms array has capacity 'need'. */
static void ensure_terms_capacity(size_t need)
{
    if (termsCap < need) {
        size_t newCap = termsCap ? termsCap * 2 : 16;
        while (newCap < need) newCap <<= 1;
        seq_t *tmp = (seq_t *)realloc(terms, newCap * sizeof(seq_t));
        terms = tmp;
        termsCap = newCap;
    }
}

/* Ensure nonprimes array has capacity 'need'. */
static void ensure_nonprime_capacity(size_t need)
{
    if (nonprimeCap < need) {
        size_t newCap = nonprimeCap ? nonprimeCap * 2 : 16;
        while (newCap < need) newCap <<= 1;
        seq_t *tmp = (seq_t *)realloc(nonprimes, newCap * sizeof(seq_t));
        nonprimes = tmp;
        nonprimeCap = newCap;
    }
}

/* Push a value into the terms array. */
static void push_term(seq_t val)
{
    ensure_terms_capacity(termsSize + 1);
    terms[termsSize++] = val;
}

/* Push a value into the nonprimes array. */
static void push_nonprime(seq_t val)
{
    ensure_nonprime_capacity(nonprimeCnt + 1);
    nonprimes[nonprimeCnt++] = val;
}

/* Ensure nonprimes array contains indices up to 'need' (size > need). */
static void generate_nonprimes_upto(idx_t need)
{
    while (nonprimeCnt <= need) {
        ++lastChecked;
        if (!is_prime(lastChecked))
            push_nonprime((seq_t)lastChecked);
    }
}

/* Ensure that nonprimes[0] == 0 is present. */
static void init_nonprimes(void)
{
    if (nonprimeCnt == 0) {
        ensure_nonprime_capacity(1);
        nonprimes[0] = (seq_t)0;
        nonprimeCnt = 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* outside defined offset */
    init_nonprimes();                /* make sure 0 is present */
    while (termsSize < (size_t)n) {
        idx_t cand = currentCandidate;
        generate_nonprimes_upto(cand);
        seq_t np = nonprimes[cand];
        bool p1 = is_prime((idx_t)np - 1);
        bool p2 = is_prime((idx_t)np + 1);
        if (p1 ^ p2)
            push_term((seq_t)cand);
        ++currentCandidate;
    }
    return terms[n - 1];
}
#endif
