/*
 * A122480  Least k>2 such that n is equal to arithmetic mean of k consecutive
 *          primes.
 * https://oeis.org/A122480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Least k>2 such that n equals the arithmetic mean of k
 *            consecutive primes
 * Range:     u8 n <= 3, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation uses a prefix‑sum sieve and incremental caching. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */

/* ---------- global static state ---------- */
static seq_t *result_cache = NULL;          /* result_cache[n] = a(n) */
static size_t result_cache_len = 0;         /* current length of result_cache */
static idx_t result_cache_nmax = 0;         /* highest n that has been computed */

static uint64_t *prime_list = NULL;         /* array of primes */
static size_t prime_count = 0;              /* number of primes in prime_list */
static idx_t prime_limit = 0;               /* last limit used to generate primes */

static seq_t *prefix_sums = NULL;           /* prefix_sums[i] = sum of first i primes, i=0..prime_count */

/* ---------- helper to rebuild primes up to at least 'limit' ---------- */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit) return;   /* already sufficient */

    /* free old prime data */
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }
    if (prefix_sums) {
        free(prefix_sums);
        prefix_sums = NULL;
    }

    /* install new primes */
    uint64_t *tmp = NULL;
    size_t cnt = 0;
    build_primes_upto(limit, &tmp, &cnt);

    prime_list  = tmp;
    prime_count = cnt;
    prime_limit = limit;

    /* build prefix sums */
    prefix_sums = (seq_t *)malloc((prime_count + 1) * sizeof(seq_t));
    prefix_sums[0] = (seq_t)0;
    for (size_t i = 0; i < prime_count; ++i) {
        prefix_sums[i + 1] = prefix_sums[i] + (seq_t)prime_list[i];
    }
}

/* ---------- helper to compute results up to a given nmax ---------- */
static void recompute_results_up_to(idx_t nmax)
{
    if (nmax <= result_cache_nmax) return;   /* already done */

    /* ensure cache large enough */
    size_t new_len = (size_t)(nmax + 1);
    if (new_len > result_cache_len) {
        seq_t *tmp = (seq_t *)realloc(result_cache, new_len * sizeof(seq_t));
        if (!tmp) { free(result_cache); result_cache = NULL; result_cache_len = 0; return; }
        for (size_t i = result_cache_len; i < new_len; ++i) tmp[i] = (seq_t)0;
        result_cache = tmp;
        result_cache_len = new_len;
    }

    /* approximate prime limit needed. For n up to 10000 this overshoots safely. */
    idx_t needed_limit = (nmax > 20) ? (nmax * 20) : 100;
    ensure_primes_upto(needed_limit);

    /* scan all windows of size k >= 3 */
    for (size_t k = 3; k <= prime_count; ++k) {
        /* the smallest possible average for this k occurs at the first window */
        seq_t first_sum = prefix_sums[k] - prefix_sums[0];
        seq_t first_avg = first_sum / (seq_t)k;
        if (first_avg > (seq_t)nmax) break;   /* all further averages for this k are >= first_avg */

        for (size_t i = 0; i + k <= prime_count; ++i) {
            seq_t sum = prefix_sums[i + k] - prefix_sums[i];
            if (sum % (seq_t)k != 0) continue;
            seq_t n_val = sum / (seq_t)k;
            if (n_val < (seq_t)1 || n_val > (seq_t)nmax) continue;
            if (result_cache[(size_t)n_val] == (seq_t)0) {
                result_cache[(size_t)n_val] = (seq_t)k;
            }
        }
    }

    result_cache_nmax = nmax;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    recompute_results_up_to(n);

    return result_cache[(size_t)n];
}
