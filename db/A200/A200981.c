/*
 * A200981  Numbers k such that the sum of non-divisors of k is prime.
 * https://oeis.org/A200981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 2856, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A200981: a(n) = nth integer k such that (k*(k+1)/2 – sigma(k)) is prime */
/* where sigma(k) is the sum of all positive divisors of k. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/sigma_of.h"
#include "../utils/isPrimeGeneric.h"

/* cached sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;  /* number of terms stored */
static size_t cache_cap = 0;  /* allocated capacity */

/* next integer to test for membership in the sequence */
static idx_t next_candidate = 1;

/* append a new term to the cache, growing the array as needed */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* return the nth term of the sequence (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, graceful fallback */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t kseq = (seq_t)next_candidate;
        seq_t sum_all   = kseq * (kseq + (seq_t)1) / 2;   /* k*(k+1)/2 */
        seq_t sigma     = sigma_of(next_candidate);      /* sum of divisors */
        seq_t nondivsum = sum_all - sigma;

        if (isPrimeGeneric(nondivsum)) {
            cache_append(kseq);
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
