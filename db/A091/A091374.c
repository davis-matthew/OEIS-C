/*
 * A091374  Number of numbers <= n having fewer prime factors than the value
 *          of their smallest prime factor.
 * https://oeis.org/A091374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{m ≤ n : bigomega(m) < spf(m)} (spfa(1)=1,
 *            bigomega(1)=0)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static idx_t *spf            = NULL;            /* smallest prime factor */
static unsigned char *bigomega = NULL;         /* Ω(m) for m ≤ limit (fits in a byte for m<2¹⁶⁴) */
static seq_t    *prefix      = NULL;             /* prefix counts A091374(1..n) */
static idx_t    precomputed_limit = 0;

static idx_t *primes          = NULL;           /* list of primes for the sieve */
static size_t primes_cnt      = 0;
static size_t primes_capacity = 0;

/* Rebuild all tables up to at least 'limit' (ε = limit+1 for array size) */
static void rebuild(idx_t limit)
{
    if (limit <= precomputed_limit) return;

    idx_t i, p, idx;
    size_t sz = (size_t)(limit + 1);

    /* reallocate spf array */
    spf = (idx_t *)realloc(spf, sz * sizeof(idx_t));
    if (!spf) exit(EXIT_FAILURE);

    /* reallocate bigomega array */
    bigomega = (unsigned char *)realloc(bigomega, sz * sizeof(unsigned char));
    if (!bigomega) exit(EXIT_FAILURE);

    /* reallocate prefix sum array */
    prefix = (seq_t *)realloc(prefix, sz * sizeof(seq_t));
    if (!prefix) exit(EXIT_FAILURE);

    /* grow primes buffer if necessary */
    if (sz > primes_capacity) {
        size_t new_cap = sz * 2;
        primes = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        if (!primes) exit(EXIT_FAILURE);
        primes_capacity = new_cap;
    }

    /* Sieve of Eratosthenes (linear) */
    spf[1] = 1;
    bigomega[1] = 0;
    primes_cnt = 0;

    for (i = 2; i <= limit; ++i) spf[i] = 0;

    for (i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = i;
            bigomega[i] = 1;
            primes[primes_cnt++] = i;
        }
        for (idx = 0; idx < primes_cnt; ++idx) {
            p = primes[idx];
            unsigned __int128 prod = (unsigned __int128)p * (unsigned __int128)i;
            if (prod > limit) break;
            spf[(idx_t)prod] = p;
            bigomega[(idx_t)prod] = bigomega[i] + 1;
            if (p == spf[i]) break;
        }
    }

    /* build prefix sums of the desired property */
    prefix[0] = 0;
    for (i = 1; i <= limit; ++i) {
        seq_t add = (bigomega[i] < spf[i]) ? (seq_t)1 : (seq_t)0;
        prefix[i] = prefix[i-1] + add;
    }

    precomputed_limit = limit;
}

/* exported function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety, not used by the judge */
    if (n > precomputed_limit) rebuild(n);
    return prefix[n];
}
