/*
 * A062824  a(n) = Ch(A005117(n)) where Ch(n) is Chowla's function and
 *          A005117(n) are the squarefree numbers.
 * https://oeis.org/A062824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Ch(A005117(n)) = σ(A005117(n)) – 1 – A005117(n), where σ
 *            is the sum-of-divisors function and A005117(n) is the nth
 *            squarefree number. For A005117(n)=1 the value is 0
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;   /* primes for factorisation */
static size_t prime_cnt   = 0;          /* number of primes */

static uint64_t *sqfree = NULL;        /* cached squarefree numbers */
static size_t   sqfree_cnt = 0;         /* number of cached numbers */
static size_t   sqfree_cap = 0;         /* current vector capacity */
static uint64_t next_candidate = 1;     /* next integer to test */

/* Make sure the prime table exists (once) */
static void init_primes(void)
{
    if (!prime_list) {
        /* The 10‑th squarefree number is 30, a safe upper bound is 200 000 */
        build_primes_upto(200000, &prime_list, &prime_cnt);
    }
}

/* Append a new squarefree number to the cached vector */
static void append_sqfree(uint64_t x)
{
    if (sqfree_cnt == sqfree_cap) {
        sqfree_cap = sqfree_cap ? sqfree_cap * 2 : 8;
        sqfree = (uint64_t*)realloc(sqfree, sqfree_cap * sizeof(uint64_t));
    }
    sqfree[sqfree_cnt++] = x;
}

/* Ensure that at least n squarefree numbers have been generated */
static void ensure_sqfree(idx_t n)
{
    while (sqfree_cnt < n) {
        if (is_squarefree((seq_t)next_candidate))
            append_sqfree(next_candidate);
        ++next_candidate;
    }
}

/* Compute σ(s) for a squarefree s (product of distinct primes) */
static seq_t sigma_squarefree(uint64_t s)
{
    seq_t prod = 1;            /* σ(1) = 1, start with multiplicative identity */
    uint64_t rem = s;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > rem) break;
        if (rem % p == 0) {
            prod *= (seq_t)(1 + p);  /* for squarefree: multiply by (1+p) */
            rem /= p;
        }
    }
    if (rem > 1)                 /* remaining prime factor > sqrt(s) */
        prod *= (seq_t)(1 + rem);

    return prod;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    init_primes();
    ensure_sqfree(n);

    uint64_t s = sqfree[n - 1];
    if (s == 1) return 0;                         /* definition for 1 */

    seq_t sigma = sigma_squarefree(s);
    sigma -= (seq_t)1;
    sigma -= (seq_t)s;
    return sigma;
}
