/*
 * A071292  Call f(n) the sum of the first n primes then a(n) is the number of
 *          squares between f(n) and f(n+1).
 * https://oeis.org/A071292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 82, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A071292
 *  a(n) = number of perfect squares k^2 with
 *         sum_{i=1..n} p_i < k^2 <= sum_{i=1..n+1} p_i,
 *  where p_i is the i-th prime, and n >= 1
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime_small.h"

/* ---- cache tables ---- */
static uint64_t *prime_cache = NULL;   /* primes, 1-based indexing (prime_cache[0] = 2) */
static size_t   prime_cache_cnt = 0;   /* number of primes stored */

static seq_t *sum_cache = NULL;       /* cumulative sums: sum_cache[0] = 0,
                                        sum_cache[i] = sum of first i primes */
static size_t   sum_cache_cnt = 0;     /* number of cumulative sums stored */

/* helper to grow an array of uint64_t */
static void grow_uint64_array(uint64_t **arr, size_t *cnt, size_t new_cnt)
{
    uint64_t *tmp = (uint64_t*)realloc(*arr, new_cnt * sizeof(uint64_t));
    /* In case realloc fails, the original block is left untouched. */
    *arr = tmp;
    *cnt = new_cnt;
}

/* helper to grow an array of seq_t */
static void grow_seq_array(seq_t **arr, size_t *cnt, size_t new_cnt)
{
    seq_t *tmp = (seq_t*)realloc(*arr, new_cnt * sizeof(seq_t));
    *arr = tmp;
    *cnt = new_cnt;
}

/* ensure at least n primes are cached  (n >= 1)  */
static void ensure_primes(idx_t n)
{
    while (prime_cache_cnt < (size_t)n) {
        uint64_t start = (prime_cache_cnt == 0) ? 2 : prime_cache[prime_cache_cnt-1] + 1;
        uint64_t p = start;
        for (;; ++p) {
            if (is_prime_small(p)) break;
        }
        size_t new_cnt = prime_cache_cnt + 1;
        grow_uint64_array(&prime_cache, &prime_cache_cnt, new_cnt);
        prime_cache[prime_cache_cnt-1] = p;
    }
}

/* ensure cumulative sums up to at least n are cached (sum_cache[i] for i =0..n)
 *  we need sums up to n+1 to answer a(n)
 */
static void ensure_sums(idx_t n)
{
    /* n is the required n (>=1). We need array indices up to n+1. */
    if (sum_cache_cnt > (size_t)(n+1))
        return;

    /* make sure primes for 1..n+1 are available */
    ensure_primes(n+1);

    size_t old_cnt = sum_cache_cnt;
    size_t new_cnt = (size_t)(n+2);               /* indices 0..n+1 inclusive */
    grow_seq_array(&sum_cache, &sum_cache_cnt, new_cnt);

    if (old_cnt == 0) {
        sum_cache[0] = (seq_t)0;
        old_cnt = 1;
    }
    for (idx_t i = old_cnt; i <= n+1; ++i) {
        seq_t add = (seq_t)prime_cache[(size_t)i-1];   /* prime i (1-indexed) */
        sum_cache[(size_t)i] = sum_cache[(size_t)i-1] + add;
    }
}

/* ---- sequence function ---- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n=0, safe */
    ensure_sums(n);
    seq_t f_n  = sum_cache[(size_t)n];
    seq_t f_np = sum_cache[(size_t)(n+1)];

    seq_t lo = floor_sqrt(f_n);
    seq_t k_min = (lo*lo == f_n) ? lo : lo + 1;
    seq_t k_max = floor_sqrt(f_np);
    if (k_min > k_max) return (seq_t)0;
    return k_max - k_min + (seq_t)1;
}
