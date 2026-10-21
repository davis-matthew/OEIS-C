/*
 * A075568  Rearrangement of noncomposite numbers such that every partial sum
 *          is composite.
 * https://oeis.org/A075568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Rearrangement of noncomposite numbers such that every partial sum is composite.
   Greedy construction: at each step pick the smallest unused noncomposite
   number that keeps the running sum composite. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"        /* is_prime_big(seq_t) */
#include "../utils/build_primes_upto.h"   /* build_primes_upto(idx_t, uint64_t**, size_t*) */

/* cache for memoized terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t produced = 0;

/* prime data */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static bool *used_prime = NULL;
static bool used_1 = false;

/* running sum */
static seq_t cumulative_sum = 0;

/* ensure list of primes up to an upper bound */
static void ensure_primes(void)
{
    if (prime_list != NULL)
        return;
    idx_t limit = (idx_t)200000;                 /* > prime(10000) */
    build_primes_upto(limit, &prime_list, &prime_count);
    used_prime = (bool*)calloc(prime_count, sizeof(bool));
}

/* grow the cache to hold at least n elements */
static void ensure_cache(size_t n)
{
    if (n <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* the sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n<1, harmless */

    size_t idx = (size_t)n;
    ensure_primes();
    ensure_cache(idx);

    while (produced < idx) {
        seq_t term;
        if (produced == 0) {                 /* first term */
            term = (seq_t)1;
            cumulative_sum = term;
            used_1 = true;
        } else {
            /* find smallest unused prime that keeps the sum composite */
            bool found = false;
            seq_t new_sum = 0;
            for (size_t i = 0; i < prime_count; ++i) {
                if (used_prime[i])
                    continue;
                seq_t p = (seq_t)prime_list[i];
                new_sum = cumulative_sum + p;
                if (!is_prime_big(new_sum)) {    /* composite */
                    term = p;
                    cumulative_sum = new_sum;
                    used_prime[i] = true;
                    found = true;
                    break;
                }
            }
            /* As conjectured, a solution always exists for the tested range.
               If none is found, return 0 (should not happen). */
            if (!found) {
                term = (seq_t)0;
                cumulative_sum = new_sum;      /* keep sum consistent */
            }
        }
        cache[produced++] = term;
    }

    return cache[idx - 1];
}
