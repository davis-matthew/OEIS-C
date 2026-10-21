/*
 * A083411  a(n) = Sum(((k-1)/2)*k!*Stirling_2(n,k),k=1..n).
 * https://oeis.org/A083411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum(((k-1)/2)*k!*Stirling_2(n,k),k=1..n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *S_cache = NULL;      /* current Stirling row S(n,k) */
static size_t S_cap = 0;
static idx_t S_row = 0;            /* n of the row stored in S_cache */

static seq_t *fact_cache = NULL;   /* factorials fact_cache[i] = i! */
static size_t fact_cap = 0;
static idx_t fact_last = 0;        /* last factorial index computed */

static seq_t *a_cache = NULL;      /* cache of a(n) values */
static size_t a_cap = 0;

/* Computes a(n) as defined in OEIS A083411. */
seq_t a(idx_t n)
{
    /* Offset is 1; values for n < 1 are not defined. */
    if (n < 1) return 0;

    /* Allocate and initialise data structures on first call. */
    if (fact_cap == 0) {
        fact_cap = 1;
        fact_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!fact_cache) abort();
        fact_cache[0] = (seq_t)1;
        fact_last = 0;
    }
    if (S_cap == 0) {
        S_cap = 1;
        S_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!S_cache) abort();
        S_cache[0] = (seq_t)1;   /* S(0,0) = 1 */
        S_row = 0;
    }
    if (a_cap == 0) {
        a_cap = 1;
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!a_cache) abort();
        a_cache[0] = (seq_t)0;
    }

    /* If we already have the answer, return it immediately. */
    if (n <= S_row) {
        return a_cache[n];
    }

    /* Compute values from the current row up to n. */
    for (idx_t cur = S_row + 1; cur <= n; ++cur)
    {
        /* Ensure factorial table large enough. */
        if ((size_t)cur + 1 > fact_cap) {
            size_t new_cap = fact_cap * 2;
            if (new_cap <= (size_t)cur + 1) new_cap = (size_t)cur + 1;
            seq_t *tmpf = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
            if (!tmpf) abort();
            fact_cache = tmpf;
            fact_cap = new_cap;
        }

        /* Compute missing factorials up to cur. */
        for (idx_t i = fact_last + 1; i <= cur; ++i) {
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        fact_last = cur;

        /* Ensure Stirling array large enough. */
        if ((size_t)cur + 1 > S_cap) {
            size_t new_capS = S_cap * 2;
            if (new_capS <= (size_t)cur + 1) new_capS = (size_t)cur + 1;
            seq_t *tmps = (seq_t *)realloc(S_cache, new_capS * sizeof(seq_t));
            if (!tmps) abort();
            S_cache = tmps;
            S_cap = new_capS;
        }

        /* Update Stirling numbers for row cur. */
        for (idx_t k = cur; k > 0; --k) {
            S_cache[k] = S_cache[k] * (seq_t)k + S_cache[k - 1];
        }
        S_cache[0] = (seq_t)0;

        /* Compute a(cur). */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t weight = ((seq_t)(k - 1) * fact_cache[k]) / (seq_t)2;
            sum += weight * S_cache[k];
        }

        /* Ensure cache large enough. */
        if ((size_t)cur >= a_cap) {
            size_t new_capA = a_cap * 2;
            if (new_capA <= (size_t)cur) new_capA = (size_t)cur + 1;
            seq_t *tmpa = (seq_t *)realloc(a_cache, new_capA * sizeof(seq_t));
            if (!tmpa) abort();
            a_cache = tmpa;
            a_cap = new_capA;
        }
        a_cache[cur] = sum;
    }

    S_row = n;
    return a_cache[n];
}
