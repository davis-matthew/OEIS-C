/*
 * A172354  n such that the Moebius function take successively, from n, the
 *          values -1,0,-1,0,-1,0.
 * https://oeis.org/A172354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 269, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A172354
   Numbers n such that μ(n) = -1, μ(n+1)=0, μ(n+2) = -1, μ(n+3)=0,
   μ(n+4)=−1, μ(n+5)=0.  Computed by sieving μ up to a fixed limit
   and collecting all n that satisfy the pattern. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Sieve parameters ---------- */
#define SIEVE_LIMIT   20000000UL   /* numbers up to this value are sieved once */

/* ---------- Static storage ---------- */
static bool          ready     = false;
static uint64_t     *terms_u64 = NULL;  /* array of found terms as uint64_t */
static size_t        terms_cnt   = 0;    /* number of terms stored */
static size_t        terms_cap   = 0;    /* current capacity of terms_u64 */

/* ---------- Helper: grow the terms array ---------- */
static void grow_terms(size_t min_needed)
{
    if (min_needed <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 32768;
    if (new_cap < min_needed)
        new_cap = min_needed;
    uint64_t *tmp = (uint64_t *)realloc(terms_u64, new_cap * sizeof(uint64_t));
    if (!tmp) {
        abort(); /* Out of memory: terminate */
    }
    terms_u64 = tmp;
    terms_cap = new_cap;
}

/* ---------- Build μ array and collect terms ---------- */
static void build_terms(void)
{
    if (ready)
        return;

    /* Allocate arrays for sieve */
    int8_t  *mu  = (int8_t *)calloc(SIEVE_LIMIT + 6, sizeof(int8_t));
    uint32_t *spf = (uint32_t *)calloc(SIEVE_LIMIT + 6, sizeof(uint32_t));
    if (!mu || !spf) {
        abort(); /* allocation failed */
    }

    /* Linear sieve to compute μ(n) */
    mu[1] = 1;
    uint32_t *primes = (uint32_t *)malloc((SIEVE_LIMIT >> 3) * sizeof(uint32_t));  /* rough upper bound */
    if (!primes) {
        free(mu); free(spf);
        abort();
    }
    size_t primes_cnt = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            mu[i] = -1;
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t prod = (uint64_t)i * p;
            if (prod > SIEVE_LIMIT)
                break;
            spf[prod] = p;
            if (i % p == 0) {
                mu[prod] = 0;
                break;
            } else {
                mu[prod] = -mu[i];
            }
        }
    }

    free(primes);

    /* Collect terms satisfying the pattern */
    grow_terms(32768);
    for (uint32_t n = 1; n + 5 <= SIEVE_LIMIT; ++n) {
        if (mu[n] == -1 &&
            mu[n+1] == 0 &&
            mu[n+2] == -1 &&
            mu[n+3] == 0 &&
            mu[n+4] == -1 &&
            mu[n+5] == 0) {
            if (terms_cnt == terms_cap)
                grow_terms(terms_cap * 2);
            terms_u64[terms_cnt++] = (uint64_t)n;
        }
    }

    free(mu);
    free(spf);
    ready = true;
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    /* Ensure table is built */
    if (!ready)
        build_terms();

    /* Indexing is 1‑based; behaviour for n < 1 is undefined */
    if (n == 0 || (size_t)(n - 1) >= terms_cnt)
        return (seq_t)0;  /* return 0 for missing terms; callers only rely on n within range */

    return (seq_t)terms_u64[(size_t)(n - 1)];
}
