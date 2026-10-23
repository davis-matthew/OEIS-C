/*
 * A152126  If f(x) = x^3+x^5+x^11+x^17+x^29+x^41+..., where the exponents are
 *          the smaller twin of twin prime pairs, consider {f(x)}^2 and write
 *          the exponents of that expansion down : x^6+2x^8+x^10+2x^12+....
 *          The proposed sequence is that sequence of exponents.
 * https://oeis.org/A152126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A152126:  a(n) = n‑th smallest integer that is the sum of two
    smaller twin primes (including the pair itself).
    The sequence consists of all distinct sums p+q with p ≤ q, where
    p,q are the smaller members of twin prime pairs.  The sums are sorted
    in increasing order.  The implementation sieves primes up to a
    dynamically chosen bound, collects the smaller twin primes, builds
    a boolean table of reachable sums, and extracts the sorted list
    into a static cache.  The cache is extended only when a requested
    index exceeds the number of precomputed terms.  All arithmetic
    is performed in the provided integer type seq_t.  */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* static cache of computed terms */
static seq_t *seq_arr = NULL;
static size_t  seq_len = 0;

/* Local helper: sieve primes up to 'limit' (inclusive),
   returns array of uint64_t primes, sets *cnt_out.  Caller must
   free the returned array. */
static void sieve_primes_up_to(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    size_t sz = (size_t)limit + 1;          /* indices 0..limit */
    bool *is_prime = (bool *)calloc(sz, sizeof(bool));
    if (!is_prime) return;                 /* allocation failed */

    /* initialize all entries >=2 to true */
    for (size_t i = 2; i < sz; ++i) is_prime[i] = true;

    for (size_t i = 2; i * i < sz; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j < sz; j += i)
                is_prime[j] = false;

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i < sz; ++i)
        if (is_prime[i]) ++cnt;

    uint64_t *primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes) { free(is_prime); return; }

    size_t idx = 0;
    for (size_t i = 2; i < sz; ++i)
        if (is_prime[i]) primes[idx++] = (uint64_t)i;

    free(is_prime);
    *primes_out = primes;
    *cnt_out = cnt;
}

/* Recompute the cache so that seq_arr contains at least 'needed' terms. */
static void recompute(idx_t needed)
{
    /* start with a modest bound; double until enough terms are obtained */
    idx_t limit = 4000;
    while (1) {
        /* collect primes up to limit+1 (so that p+2 <= limit+1 works) */
        uint64_t *primes = NULL;
        size_t prime_cnt = 0;
        sieve_primes_up_to(limit + 1, &primes, &prime_cnt);
        if (!primes) { /* allocation failed – bail out */ return; }

        /* gather smaller twin primes */
        uint64_t *twin = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
        if (!twin) { free(primes); return; }
        size_t twin_len = 0;
        for (size_t i = 0; i + 1 < prime_cnt; ++i)
            if (primes[i] + 2 == primes[i + 1])
                twin[twin_len++] = primes[i];
        free(primes);

        /* mark all reachable sums p+q (p <= q) */
        size_t sum_size = (size_t)limit * 2 + 1;
        bool *present = (bool *)calloc(sum_size, sizeof(bool));
        if (!present) { free(twin); return; }

        for (size_t i = 0; i < twin_len; ++i) {
            uint64_t pi = twin[i];
            for (size_t j = i; j < twin_len; ++j) {
                uint64_t sum = pi + twin[j];
                if (sum < sum_size) present[sum] = true;
            }
        }

        /* count distinct sums */
        size_t new_len = 0;
        for (size_t v = 0; v < sum_size; ++v)
            if (present[v]) ++new_len;

        if (new_len >= (size_t)needed) {
            /* allocate array for new terms */
            seq_t *new_arr = (seq_t *)malloc(sum_size * sizeof(seq_t));
            if (!new_arr) { free(twin); free(present); return; }

            size_t k = 0;
            for (size_t v = 0; v < sum_size; ++v)
                if (present[v]) new_arr[k++] = (seq_t)v;   /* k == new_len */

            /* replace old cache */
            free(seq_arr);
            seq_arr = new_arr;
            seq_len = k;           /* k == new_len */

            /* clean up */
            free(twin);
            free(present);
            break;                  /* succeeded */
        }

        /* not enough terms – free temporary data and increase limit */
        free(twin);
        free(present);
        limit <<= 1;                /* double the limit and retry */
    }
}

/* --------------------------------------------------------------------- */
/* public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n = 0 */
    if (seq_len < (size_t)n)          /* need more terms */
        recompute(n);
    return seq_arr[n - 1];
}
