/*
 * A098013  Differences between consecutive primes that are twice primes.
 * https://oeis.org/A098013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 3563, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098013: a(n) = the difference between consecutive primes
 * that is twice a prime (computed by a small sieve / search). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"   /* bool is_prime_small(uint64_t) */

/* Cached differences and state for incremental generation */
static seq_t *gap_cache   = NULL;
static size_t   gap_cnt   = 0;
static size_t   gap_cap   = 0;

/* State of the prime generator */
static uint64_t last_prime       = 2;   /* last prime processed */
static uint64_t next_candidate   = 3;   /* next odd number to test */

/* Ensure gap cache has room for 'need' elements */
static void ensure_gap_cap(size_t need)
{
    if (gap_cap >= need) return;
    size_t new_cap = gap_cap ? gap_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(gap_cache, new_cap * sizeof(seq_t));
    gap_cache = tmp;
    gap_cap   = new_cap;
}

/* Generate terms until we have at least 'n' gaps stored */
static void generate_terms(idx_t n)
{
    while (gap_cnt < (size_t)n) {
        /* Find next prime */
        while (!is_prime_small(next_candidate))
            next_candidate += 2;

        uint64_t cur_prime = next_candidate;
        next_candidate += 2;                /* advance to the next odd */

        /* Compute gap from previous prime */
        uint64_t diff = cur_prime - last_prime;
        if ((diff & 1u) == 0) {              /* even gap only can be 2*prime */
            uint64_t half = diff / 2;
            if (is_prime_small(half)) {
                ensure_gap_cap(gap_cnt + 1);
                gap_cache[gap_cnt] = (seq_t)diff;
                gap_cnt++;
            }
        }

        last_prime = cur_prime;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* offset defined as 1 */
    generate_terms(n);
    return gap_cache[(size_t)n - 1];
}
