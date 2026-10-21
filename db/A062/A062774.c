/*
 * A062774  Inverse Moebius transform of PrimePi function.
 * https://oeis.org/A062774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} pi(d) , pi(d) = number of primes <= d
 * Range:     u8 n <= 335, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function can be called in any order.  For each call we expand
   the internal sieve and prime‑count table up to the requested
   n, then compute the sequence values by
   a[n] = sum_{d|n} pi(d).  The computation is O(n log n) for the
   largest n seen, which is trivial for n <= 10000. */

#include <stdlib.h>
#include <stdbool.h>

static idx_t max_limit = 0;
static uint8_t *is_prime   = NULL; /* 0 = composite, 1 = prime */
static idx_t *prime_cnt   = NULL;  /* prime count up to index */
static seq_t *a_cache     = NULL;  /* a[n] values */

/* Ensure the tables are built for all indices up to 'limit'. */
static void ensure_tables(idx_t limit) {
    if (limit <= max_limit) return;

    /* Free any previous tables */
    free(is_prime);
    free(prime_cnt);
    free(a_cache);

    size_t size = (size_t)limit + 1;

    /* Allocate new tables */
    is_prime  = (uint8_t *)malloc(size);
    prime_cnt = (idx_t   *)malloc(size * sizeof(idx_t));
    a_cache   = (seq_t   *)malloc(size * sizeof(seq_t));
    if (!is_prime || !prime_cnt || !a_cache) return; /* no memory */

    size_t i, j;
    for (i = 0; i < size; ++i) is_prime[i] = 1;
    if (size > 0) { is_prime[0] = 0; }
    if (size > 1) { is_prime[1] = 0; }

    /* Sieve primes up to limit */
    for (i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Compute prefix prime counts */
    idx_t count = 0;
    for (i = 0; i <= limit; ++i) {
        if (is_prime[i]) ++count;
        prime_cnt[i] = count;
    }

    /* Compute a[n] by adding pi[d] to each multiple of d */
    for (i = 0; i <= limit; ++i) a_cache[i] = 0;          /* init */
    for (i = 1; i <= limit; ++i) {
        seq_t pi = (seq_t)prime_cnt[i];                  /* pi(i) */
        for (j = i; j <= limit; j += i)
            a_cache[j] += pi;
    }

    max_limit = limit;
}

seq_t a(idx_t n) {
    if (n < 1) return 0; /* undefined, return 0 as safety */
    ensure_tables(n);
    return a_cache[n];
}
