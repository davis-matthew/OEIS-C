/*
 * A048598  Partial sums of the sequence (A001097) of twin primes.
 * https://oeis.org/A048598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 101, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
   Partial sums of the sequence (A001097) of twin‑prime primes.
   a(n) = p1 + p2 + … + pn, where p1, p2, … are the primes that
   belong to a twin‑prime pair, listed in ascending order (duplicates
   are allowed, e.g. 5 appears twice because it forms the pair (3,5)
   and (5,7)).
*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* forward declaration */
static void recompute(uint64_t limit);

/* static state */
static seq_t *cum_sum = NULL;
static size_t cum_len = 0;          /* number of entries in cum_sum (incl. index 0) */
static uint64_t cur_limit = 0;      /* current sieve limit */

/* generate primes up to 'limit', collect all twin‑prime primes, and
   build the cumulative sum array cum_sum[0..n] (cum_sum[0] = 0).   */
static void recompute(uint64_t limit)
{
    /* sieve */
    uint8_t *is_prime = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!is_prime) abort();
    for (uint64_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (uint64_t q = p * p; q <= limit; q += p) is_prime[q] = 0;
        }
    }

    /* collect primes that are part of a twin pair (both elements) */
    size_t cap = limit / 20 + 10;                /* rough initial capacity */
    uint32_t *twin = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!twin) abort();
    size_t twin_len = 0;

    for (uint64_t p = 2; p <= limit; ++p) {
        if (!is_prime[p]) continue;
        bool in_pair = false;
        if (p > 2 && is_prime[p - 2]) in_pair = true;
        if (p + 2 <= limit && is_prime[p + 2]) in_pair = true;
        if (!in_pair) continue;

        if (twin_len == cap) {
            cap *= 2;
            twin = (uint32_t *)realloc(twin, cap * sizeof(uint32_t));
            if (!twin) abort();
        }
        twin[twin_len++] = (uint32_t)p;
    }

    /* build cumulative sums */
    size_t new_len = twin_len + 1;          /* include sentinel 0 */
    seq_t *new_cum = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_cum) abort();
    new_cum[0] = 0;
    for (size_t i = 1; i < new_len; ++i) {
        new_cum[i] = new_cum[i - 1] + (seq_t)twin[i - 1];
    }

    /* free old data */
    if (cum_sum) free(cum_sum);
    free(twin);
    free(is_prime);

    cum_sum = new_cum;
    cum_len = new_len;
    cur_limit = limit;
}

/* ensure that cum_sum contains at least n+1 entries */
static void ensure_upto(idx_t n)
{
    /* initial limit: enough for about 10 000 twin primes */
    const uint64_t INITIAL_LIMIT = 4000000ULL;
    if (!cum_sum) {
        recompute(INITIAL_LIMIT);
    }
    while ((idx_t)cum_len <= n) {
        /* double the sieve range until we have enough terms */
        uint64_t new_limit = cur_limit * 2 + 100;
        recompute(new_limit);
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;         /* undefined, but safe */
    ensure_upto(n);
    return cum_sum[(size_t)n];
}
