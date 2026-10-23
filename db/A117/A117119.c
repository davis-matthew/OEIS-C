/*
 * A117119  Number of partitions of 2*n into two odd prime powers.
 * https://oeis.org/A117119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ odd prime powers x,y : x + y = 2*n , 1 ≤ x ≤ y }
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* compare function for qsort on uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    return (x[0] < y[0]) ? -1 : (x[0] > y[0]) ? 1 : 0;
}

/* ------------------------------------------------------------------ */
/* Generate all primes ≤ limit into primes_out (array of uint64_t)     */
/* The array is allocated with malloc and must be freed by the caller. */
/* ------------------------------------------------------------------ */
static void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    /* Simple sieve of Eratosthenes */
    /* Allocate a byte per number (0 = composite, 1 = prime) */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)limit + 1);
    if (!is_prime) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes to know how many to allocate */
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++prime_cnt;

    uint64_t *primes = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            primes[idx++] = (uint64_t)i;
        }
    }

    free(is_prime);
    *primes_out = primes;
    *cnt_out = prime_cnt;
}

/* ------------------------------------------------------------------ */
/* Global memoised data                                               */
/* ------------------------------------------------------------------ */
static seq_t      *counts      = NULL;           /* counts[0 .. max_n_current] */
static uint64_t   *prime_pows  = NULL;           /* odd prime powers ≤ 2*max_n_current */
static size_t      pp_cnt      = 0;              /* number of prime powers   */
static idx_t       max_n_current = 0;           /* maximum n that is ready  */

/* ------------------------------------------------------------------ */
/* recompute the sequence up to new_max_n (inclusive)                 */
/* ------------------------------------------------------------------ */
static void recompute(idx_t new_max_n)
{
    idx_t  limit = new_max_n * 2ULL;  /* maximum sum 2*n */

    /* ---- build primes up to limit --------------------------------*/
    uint64_t *primes = NULL;
    size_t    pr_cnt = 0;
    build_primes_upto(limit, &primes, &pr_cnt);

    /* ---- generate odd prime powers (including 1) -----------------*/
    /* estimate size: at most 3 times the number of primes (powers per prime) */
    size_t est_size = pr_cnt * 3 + 1;
    uint64_t *tmp   = (uint64_t *)malloc(est_size * sizeof(uint64_t));
    size_t   tmp_n  = 0;

    /* insert 1 once */
    tmp[tmp_n++] = 1ULL;

    for (size_t i = 0; i < pr_cnt; ++i) {
        uint64_t p = primes[i];
        if (p == 2ULL) continue;                /* skip even prime */
        uint64_t val = p;
        while (val <= limit) {
            if (tmp_n >= est_size) {
                est_size *= 2;
                tmp = (uint64_t *)realloc(tmp, est_size * sizeof(uint64_t));
            }
            tmp[tmp_n++] = val;
            if (val > limit / p) break;         /* avoid overflow */
            val *= p;
        }
    }
    free(primes);

    /* ---- sort the list -------------------------------------------*/
    qsort(tmp, tmp_n, sizeof(uint64_t), cmp_uint64);

    /* ---- store into prime_pows -----------------------------------*/
    free(prime_pows);
    prime_pows = (uint64_t *)malloc(tmp_n * sizeof(uint64_t));
    if (!prime_pows) {
        /* allocation failed; keep counts zeroed */
        pp_cnt = 0;
        free(tmp);
        return;
    }
    memcpy(prime_pows, tmp, tmp_n * sizeof(uint64_t));
    pp_cnt = tmp_n;
    free(tmp);

    /* ---- allocate count array -------------------------------------*/
    free(counts);
    counts = (seq_t *)calloc((size_t)(new_max_n + 1), sizeof(seq_t));
    if (!counts) {
        /* allocation failed */
        pp_cnt = 0;
        return;
    }

    /* ---- count unordered pairs -----------------------------------*/
    for (size_t i = 0; i < pp_cnt; ++i) {
        uint64_t x = prime_pows[i];
        for (size_t j = i; j < pp_cnt; ++j) {
            uint64_t y = prime_pows[j];
            uint64_t s = x + y;
            if (s > limit) break;                  /* further y increase sum */
            if (s & 1ULL) continue;                /* sum must be even    */
            idx_t n = s >> 1;                      /* n = s/2             */
            if (n == 0 || n > new_max_n) continue;
            counts[n]++;                            /* count in seq_t      */
        }
    }
    max_n_current = new_max_n;
}

/* ------------------------------------------------------------------ */
/* required public function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                          /* undefined for n=0   */
    if (n > max_n_current) {
        recompute(n);                               /* extend storage      */
    }
    return counts[n];
}
