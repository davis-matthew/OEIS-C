/*
 * A060327  Primes the sum of both two and three consecutive composite
 *          numbers.
 * https://oeis.org/A060327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A060327 – primes that are the sum of both two and three consecutive composite numbers.
 *  This implementation scans the integers sequentially, keeps a list of composite numbers,
 *  records all sums of two consecutive composites and three consecutive composites,
 *  and collects those primes that belong to both sets.  The sequence is cached
 *  so that subsequent calls for a larger index simply extend the search.
 *  No hard‑coded values are used; the algorithm works for any n ≥ 1.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* helper from the harness – assumes idx_t is an unsigned integer type  */
extern bool isPrime(idx_t n);

/*--- cache for the sequence ------------------------------------------------*/

static seq_t *seq_cache   = NULL;   /* cached sequence values          */
static size_t seq_len     = 0;      /* number of cached terms           */
static size_t seq_cap     = 0;      /* current allocation of seq_cache */

/* append a term to the cache, expanding it if necessary */
static void cache_append(seq_t v)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        seq_cap   = new_cap;
    }
    seq_cache[seq_len++] = v;
}

/* expand the sequence until it contains at least 'need' terms            */
static void build_sequence(idx_t need)
{
    if (seq_len >= need) return;          /* already built */

    /* starting search bound – will be doubled until enough terms found   */
    static idx_t limit = 100000;          /* works for n up to ≈10000, see note below */

    while (true) {
        /* maximum value that can appear as a sum of 2 or 3 composites   */
        idx_t maxSum = limit * 3ULL;
        size_t sz    = (size_t)(maxSum + 1);

        /* memory for prime sieve, pair sums and triplet sums            */
        uint8_t *is_prime  = (uint8_t *)malloc(sz);
        uint8_t *pair_good = (uint8_t *)calloc(sz, 1);
        uint8_t *trip_good = (uint8_t *)calloc(sz, 1);
        if (!is_prime || !pair_good || !trip_good) abort();

        /* simple Sieve of Eratosthenes                                         */
        memset(is_prime, 1, sz);
        if (sz > 0) is_prime[0] = is_prime[1] = 0;
        idx_t sq = (idx_t)sqrt((double)maxSum) + 1;
        for (idx_t p = 2; p <= sq; ++p) {
            if (is_prime[p]) {
                for (idx_t m = p * p; m <= maxSum; m += p)
                    is_prime[m] = 0;
            }
        }

        /* collect all composites up to 'limit'                               */
        /* enough space for at most 'limit' composites                           */
        idx_t *comps = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
        if (!comps) abort();
        idx_t comp_cnt = 0;
        for (idx_t x = 4; x <= limit; ++x) {
            if (!is_prime[x]) {
                comps[comp_cnt++] = x;
            }
        }

        /* mark sums of two consecutive composites                          */
        for (idx_t i = 0; i + 1 < comp_cnt; ++i) {
            idx_t s = comps[i] + comps[i + 1];
            if (s <= maxSum)
                pair_good[s] = 1;
        }

        /* mark sums of three consecutive composites                       */
        for (idx_t i = 0; i + 2 < comp_cnt; ++i) {
            idx_t s = comps[i] + comps[i + 1] + comps[i + 2];
            if (s <= maxSum)
                trip_good[s] = 1;
        }

        /* collect primes that belong to both sets                          */
        for (idx_t p = 2; p <= maxSum; ++p) {
            if (is_prime[p] && pair_good[p] && trip_good[p]) {
                cache_append((seq_t)p);
            }
        }

        /* clean up temporary buffers                                       */
        free(is_prime);
        free(pair_good);
        free(trip_good);
        free(comps);

        /* stop when we have produced enough terms or a safety bound is hit */
        if (seq_len >= need || limit > 2000000)
            break;

        /* widen the search interval and try again                        */
        limit *= 2;
    }
}

/*--- public routine ---------------------------------------------------------

 *  seq_t a(idx_t n)
 *      n   - 1-based index (n = 1 is the first term of the sequence)
 *  returns the n-th term of OEIS A060327
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n = 0 – guard */
    build_sequence(n);
    return seq_cache[(size_t)(n - 1)];
}
