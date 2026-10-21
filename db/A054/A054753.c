/*
 * A054753  Numbers which are the product of a prime and the square of a
 *          different prime (p^2 * q).
 * https://oeis.org/A054753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A054753: numbers of the form p^2 * q, where p and q are
   distinct primes.  The function computes the n‑th term by generating
   all such products up to a dynamically chosen bound, sorting them,
   and caching the results for later calls.  This is a simple search
   approach that runs well below 2 s for n ≤ 10 000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
   Helpers
   ------------------------------------------------------------------ */

/* Compare two seq_t values for qsort.  The type seq_t supports
   the < operator. */
static int cmp_seq_t(const void *pa, const void *pb)
{
    const seq_t *a = (const seq_t *)pa;
    const seq_t *b = (const seq_t *)pb;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Simple estimate of an upper bound for the value of the n-th term.
   For n ≤ 10 000 a bound of n*20+1000 is ample (≈2.2 n log n). */
static size_t estimate_limit(idx_t n)
{
    if (n == 0) return 20;
    return (size_t)n * 20 + 1000;
}

/* Generate all primes <= limit and return them in an array.
   *primes is set to point to a malloc'ed array, *cnt to its size.
   The caller must free(*primes). */
static void sieve_primes(size_t limit, uint32_t **primes, size_t *cnt)
{
    unsigned char *is_comp = (unsigned char *)calloc(limit + 1, 1);
    if (!is_comp) { *primes = NULL; *cnt = 0; return; }

    size_t i, j;
    for (i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (j = i * i; j <= limit; j += i) is_comp[j] = 1;
        }
    }

    size_t cap = limit / 10 + 10;
    uint32_t *tmp = (uint32_t *)malloc(cap * sizeof(uint32_t));
    size_t npr = 0;
    for (i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (npr == cap) {
                cap <<= 1;
                tmp = (uint32_t *)realloc(tmp, cap * sizeof(uint32_t));
            }
            tmp[npr++] = (uint32_t)i;
        }
    }
    free(is_comp);
    *primes = tmp;
    *cnt = npr;
}

/* Generate all numbers <= limit of the form p^2 * q (p != q) and store
   them in a sorted array.  *vals receives the array and *cnt its size.
   The caller must free(*vals). */
static void generate_terms(size_t limit, seq_t **vals, size_t *cnt)
{
    uint32_t *primes = NULL;
    size_t prime_cnt = 0;
    sieve_primes(limit, &primes, &prime_cnt);
    if (!primes) { *vals = NULL; *cnt = 0; return; }

    size_t cap = 65536;
    seq_t *buffer = (seq_t *)malloc(cap * sizeof(seq_t));
    size_t nvals = 0;

    for (size_t ip = 0; ip < prime_cnt; ++ip) {
        uint32_t p = primes[ip];
        uint64_t p2 = (uint64_t)p * (uint64_t)p;
        if (p2 > limit) break;
        for (size_t iq = 0; iq < prime_cnt; ++iq) {
            if (iq == ip) continue;
            uint32_t q = primes[iq];
            uint64_t prod = p2 * (uint64_t)q;
            if (prod > limit) break;          /* primes are increasing   */
            if (nvals == cap) {
                cap <<= 1;
                buffer = (seq_t *)realloc(buffer, cap * sizeof(seq_t));
            }
            buffer[nvals++] = (seq_t)prod;
        }
    }

    free(primes);
    qsort(buffer, nvals, sizeof(seq_t), cmp_seq_t);
    *vals = buffer;
    *cnt = nvals;
}

/* ------------------------------------------------------------------
   Public sequence function
   ------------------------------------------------------------------ */

/* Compute the n-th term of A054753.  Cache is reused for successive
   calls; the array contains at least n terms when returned. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but harmless */

    static seq_t *cache = NULL;
    static size_t cache_size = 0;    /* number of cached terms */

    /* Return from cache if we already have enough terms. */
    if (cache != NULL && (size_t)n <= cache_size) {
        return cache[n - 1];
    }

    /* Generate terms until we have at least n of them. */
    size_t limit = estimate_limit(n);
    while (1) {
        seq_t *vals = NULL;
        size_t cnt = 0;
        generate_terms(limit, &vals, &cnt);
        if (cnt >= (size_t)n) {
            free(cache);         /* discard previous cache */
            cache = vals;
            cache_size = cnt;
            return cache[n - 1];
        }
        free(vals);
        limit <<= 1;             /* double the bound and retry */
    }
}
