/*
 * A179789  Sum of the differences between the first prime and the next,
 *          inside in the first jump in a Sieve of Eratosthenes table.
 * https://oeis.org/A179789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p_k > p_n, p_k < 2·p_n} (p_k – p_n) where p_i is the
 *            (i‑th) prime (p_0 = 2). The function is memoised with a static
 *            prime table and prefix sums so that any call order is supported
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* static cache for primes (uint64_t) and prefix sums (seq_t) */
static uint64_t *prime_list       = NULL;   /* primes[0] = 2, primes[1] = 3, … */
static size_t   prime_count      = 0;       /* number of primes in prime_list */
static seq_t   *prime_prefix     = NULL;   /* prefix[i] = sum of primes[0..i-1] */
static idx_t    prime_limit      = 0;       /* largest number that was sieved */

/* -------------------------------------------------------------------- */
/* Rebuild prime table up to at least LIMIT (inclusive).                 */
/* This discards the old cache – safe because calls are memoised   */
/* -------------------------------------------------------------------- */
static void extend_primes_to_limit(idx_t LIMIT)
{
    if (LIMIT <= prime_limit)
        return;                                      /* already sufficient */

    /* free old arrays */
    free(prime_list);
    free(prime_prefix);
    prime_list   = NULL;
    prime_prefix = NULL;
    prime_count  = 0;

    /* build primes up to LIMIT using the supplied helper */
    size_t cnt = 0;
    uint64_t *new_list = NULL;
    build_primes_upto(LIMIT, &new_list, &cnt);

    prime_list   = new_list;
    prime_count  = cnt;
    prime_limit  = LIMIT;

    /* build prefix sums of type seq_t */
    prime_prefix = (seq_t*)malloc((prime_count + 1) * sizeof(seq_t));
    if (!prime_prefix) { /* caller will not check memory failure – abort */
        abort();
    }
    prime_prefix[0] = (seq_t)0;
    for (size_t i = 0; i < prime_count; ++i)
        prime_prefix[i + 1] = prime_prefix[i] + (seq_t)prime_list[i];
}

/* -------------------------------------------------------------------- */
/* Ensure that the cache contains at least (n+1) primes and that there
   exist primes up to at least 2 * prime[n].  The simple estimator
   n*20 is more than enough for n up to 10000.                                */
/* -------------------------------------------------------------------- */
static void ensure_cache_for_n(idx_t n)
{
    /* estimate limit for nth prime */
    idx_t est = (n > 5) ? (20 * n) : 15;
    /* extend until we have n+1 primes */
    while (prime_count <= n) {
        est = (prime_count > 5) ? (20 * prime_count) : 15;
        extend_primes_to_limit(est);
    }
    /* ensure we have primes up to 2 * prime[n] */
    idx_t target = 2 * prime_list[n];
    if (target > prime_limit)
        extend_primes_to_limit(target);
}

/* -------------------------------------------------------------------- */
/* Binary search for the first index in prime_list with value >= target */
/* -------------------------------------------------------------------- */
static size_t first_index_at_or_above(uint64_t target)
{
    size_t low  = 0;
    size_t high = prime_count;          /* one past the last element */
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (prime_list[mid] < target)
            low = mid + 1;
        else
            high = mid;
    }
    return low;                         /* may equal prime_count */
}

/* -------------------------------------------------------------------- */
/* Main sequence function                                                    */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Prepare cache */
    ensure_cache_for_n(n);

    uint64_t p_n      = prime_list[n];
    uint64_t target   = 2 * p_n;
    /* Find first prime >= 2*p_n */
    size_t m_index    = first_index_at_or_above(target);
    /* indices with primes < 2*p_n are [n+1 .. m_index-1] */
    size_t count = (m_index > n + 1) ? (m_index - (n + 1)) : 0;
    seq_t sum_primes = (m_index > n + 1) ?
                       (prime_prefix[m_index] - prime_prefix[n + 1]) :
                       (seq_t)0;
    seq_t diff_sum   = sum_primes - (seq_t)count * (seq_t)p_n;
    return diff_sum;
}
