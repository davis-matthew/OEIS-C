/*
 * A137876  a(n) = (nextprime(18n)-previousprime(18n))/2.
 * https://oeis.org/A137876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (nextprime(18n)-previousprime(18n))/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Internal prime table */
static bool          sieve_done   = false;
static uint64_t      *prime_tbl    = NULL;
static size_t        prime_cnt    = 0;
static size_t        sieve_limit  = 0;

/* Build a simple sieve up to sieve_limit and store primes */
static void init_sieve(void)
{
    if (sieve_done) return;

    /* safety limit for the task (covers n up to ~10000) */
    sieve_limit = 200000;
    bool *mark = (bool *)malloc((sieve_limit + 1) * sizeof(bool));
    if (!mark) return;                   /* cannot allocate – fallback */

    for (size_t i = 0; i <= sieve_limit; ++i) mark[i] = true;
    mark[0] = mark[1] = false;

    for (size_t i = 2; i * i <= sieve_limit; ++i) {
        if (mark[i]) {
            for (size_t j = i * i; j <= sieve_limit; j += i)
                mark[j] = false;
        }
    }

    /* count primes */
    prime_cnt = 0;
    for (size_t i = 2; i <= sieve_limit; ++i)
        if (mark[i]) ++prime_cnt;

    /* copy to prime_tbl */
    prime_tbl = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    if (!prime_tbl) {
        free(mark);
        return;
    }
    size_t idx = 0;
    for (size_t i = 2; i <= sieve_limit; ++i) {
        if (mark[i]) prime_tbl[idx++] = i;
    }

    free(mark);
    sieve_done = true;
}

/* Binary search for first element >= val */
static inline size_t upper_bound(uint64_t *arr, size_t n, uint64_t val)
{
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        if (arr[mid] >= val) hi = mid;
        else                 lo = mid + 1;
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure prime table is ready */
    init_sieve();
    if (!sieve_done || !prime_tbl) return 0;  /* safety fallback */

    /* Compute 18*n */
    uint64_t m = (uint64_t)n * 18ULL;

    /* Find previous prime < 18n */
    size_t idx_ge_m = upper_bound(prime_tbl, prime_cnt, m);
    /* guard against out‑of‑range; for the given limits this never happens */
    if (idx_ge_m == 0) return 0;
    uint64_t prev_prime = prime_tbl[idx_ge_m - 1];

    /* Find next prime > 18n */
    size_t idx_gt_m = upper_bound(prime_tbl, prime_cnt, m + 1ULL);
    if (idx_gt_m >= prime_cnt) return 0;  /* should not occur for the limits */
    uint64_t next_prime = prime_tbl[idx_gt_m];

    /* compute (next - previous) / 2 */
    seq_t diff = (seq_t)next_prime - (seq_t)prev_prime;
    return diff / 2;
}
