/*
 * A239440  The minimal value of A001414(i) where prime(n) < i < prime(n+1).
 * https://oeis.org/A239440
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the minimal sum of prime factors with multiplicity among
 *            the integers that lie strictly between the nth and (n+1)th
 *            primes
 * Range:     u8 n <= 264, u16 n <= 9795, u32 n <= 9795, u64 n <= 9795,
 *            u128 n <= 9795, big any n
 * Helpers:   _prime_limit_for_idx, build_primes, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes.h"
#include "../utils/build_spf.h"

static seq_t *min_cache = NULL;      /* a(n) values  (index 0 unused) */
static size_t min_cap   = 0;         /* capacity of min_cache      */
static idx_t   max_n_computed = 1;   /* highest n for which a(n) known */

static idx_t   *prime_list = NULL; /* primes up to current limit            */
static size_t  prime_cnt   = 0;      /* number of primes in prime_list        */
static idx_t   prime_limit = 0;     /* maximum value covered by prime_list    */

static idx_t   *spf = NULL;         /* smallest prime factor for each num    */
static seq_t   *sum  = NULL;         /* sum of prime factors for each num     */
static idx_t   sum_limit = 0;        /* maximum value covered by sum array     */

/* Extend all tables so that at least max_size primes are available */
static void
extend_tables(idx_t max_size)
{
    /* Determine a sieve limit that guarantees at least max_size primes. */
    idx_t limit = _prime_limit_for_idx(max_size + 10);
    if (limit <= prime_limit) return;   /* already enough */

    /* Free old data */
    free(prime_list);
    free(spf);
    free(sum);

    /* Allocate new prime list and compute primes up to the desired limit. */
    build_primes(limit, &prime_list, &prime_cnt);

    /* Build smallest prime factor array up to the limit. */
    build_spf(limit, &spf);

    /* Build sum-of-prime-factors array. */
    sum = (seq_t *)calloc((size_t)limit + 1, sizeof(seq_t));
    sum[1] = 0;
    for (idx_t i = 2; i <= limit; ++i)
        sum[i] = sum[i / spf[i]] + (seq_t)spf[i];

    prime_limit = limit;
    sum_limit   = limit;

    /* Allocate or enlarge the cache for a(n) values. */
    if (min_cap < (size_t)prime_cnt + 5) {
        seq_t *new_cache = (seq_t *)realloc(min_cache,
                                            ((size_t)prime_cnt + 5) * sizeof(seq_t));
        if (!new_cache) abort();
        min_cache = new_cache;
        min_cap   = (size_t)prime_cnt + 5;
    }
}

/* Compute a(n) for all n up to target_n */
static void
ensure_computed(idx_t target_n)
{
    if (target_n <= max_n_computed) return;

    /* Make sure we have enough primes to handle n+1. */
    extend_tables(target_n + 2);

    for (idx_t n = max_n_computed + 1; n <= target_n; ++n) {
        /* prime indices: prime(n) = prime_list[n-1], prime(n+1) = prime_list[n] */
        idx_t  p_n   = prime_list[n - 1];
        idx_t  p_np1 = prime_list[n];   /* always exists because we built enough */
        idx_t  start = p_n + 1;
        idx_t  end   = p_np1 - 1;

        /* Find minimum sum in the range [start, end] */
        seq_t min_val = 0;
        bool first = true;
        for (idx_t i = start; i <= end; ++i) {
            seq_t v = sum[i];
            if (first || v < min_val) {
                min_val = v;
                first = false;
            }
        }

        min_cache[n] = min_val;
    }

    max_n_computed = target_n;
}

seq_t
a(idx_t n)
{
    if (n < 2) return 0;
    ensure_computed(n);
    return min_cache[n];
}
