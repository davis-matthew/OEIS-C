/*
 * A061841  Numbers k such that (k+5)^5 | k!.
 * https://oeis.org/A061841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 115, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that (k+5)^5 divides k!   */
/* The algorithm scans integers k = 1,2,…, and keeps a cache of the */
/* first terms it finds. For each candidate k it factors k+5 using a */
/* pre‑computed smallest‑prime‑factor (SPF) table and checks that */
/* Legendre's exponent v_p(k!) is at least 5 times the exponent of p */
/* in k+5. A linear sieve builds the SPF table up to a fixed size */
/* that comfortably covers the first 10 000 terms.                  */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Parameters and global state                                         */
/* ------------------------------------------------------------------ */
#define MAX_SIEVE 2000000   /* large enough for n ≤ 10 000 */

static int *spf = NULL;      /* smallest prime factor table */
static size_t spf_size = 0;  /* allocated size of spf */

/* cache of found k (index type only) */
static idx_t *k_cache = NULL;
static size_t k_cache_len = 0;
static size_t k_cache_cap = 0;

/* ------------------------------------------------------------------ */
/* Sieve utilities                                                    */
/* ------------------------------------------------------------------ */
static void cache_grow(size_t new_len)
{
    if (k_cache_len >= new_len) return;
    size_t new_cap = k_cache_cap ? k_cache_cap : 8;
    while (new_cap < new_len) new_cap <<= 1;
    idx_t *new_mem = (idx_t *)realloc(k_cache, new_cap * sizeof(idx_t));
    if (!new_mem) return;                /* allocation failure – keep old cache */
    k_cache = new_mem;
    k_cache_cap = new_cap;
}

/* initialise SPF sieve up to MAX_SIEVE */
static void init_sieve(void)
{
    if (spf != NULL) return;
    spf_size = MAX_SIEVE + 1;
    spf = (int *)malloc(spf_size * sizeof(int));
    if (!spf) return;                    /* allocation failure – caller will fail */

    for (size_t i = 0; i < spf_size; ++i) spf[i] = 0;
    if (spf_size > 1) spf[1] = 1;

    /* linear sieve */
    size_t *pr = (size_t *)malloc(spf_size * sizeof(size_t));
    size_t pcnt = 0;
    for (size_t i = 2; i < spf_size; ++i) {
        if (spf[i] == 0) {
            spf[i] = (int)i;
            if (pcnt < spf_size) pr[pcnt++] = i;
        }
        for (size_t j = 0; j < pcnt; ++j) {
            size_t p = pr[j];
            if (p > spf[i] || i * p >= spf_size) break;
            spf[i * p] = (int)p;
            if (p == (size_t)spf[i]) break;
        }
    }
    free(pr);
}

/* ------------------------------------------------------------------ */
/* Legendre exponent: v_p(n!)                                          */
/* ------------------------------------------------------------------ */
static unsigned long long legendre_exp(idx_t n, int p)
{
    unsigned long long e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* ------------------------------------------------------------------ */
/* Test whether a candidate k satisfies (k+5)^5 | k!                    */
/* ------------------------------------------------------------------ */
static bool candidate_ok(idx_t k)
{
    idx_t m = k + 5;
    while (m > 1) {
        int p = spf[(size_t)m];
        int e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        if (legendre_exp(k, p) < 5ULL * e) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but return 0 */

    init_sieve();

    idx_t next_k = k_cache_len > 0 ? k_cache[k_cache_len - 1] + 1 : 1;

    while (k_cache_len < n) {
        if (candidate_ok(next_k)) {
            cache_grow(k_cache_len + 1);
            k_cache[k_cache_len++] = next_k;
        }
        ++next_k;
        /* the pre‑allocated sieve is assumed large enough for required N */
    }
    return (seq_t)k_cache[n - 1];
}
