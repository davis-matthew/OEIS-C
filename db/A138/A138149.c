/*
 * A138149  n-th run has length n-th prime, with values 0 and 1 only, starting
 *          with 1.
 * https://oeis.org/A138149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is 1 if n lies in an even‑indexed run of ones (starting
 *            count at 0), otherwise 0. The runs alternate, each having length
 *            equal to the corresponding prime number
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static uint64_t *primes     = NULL;  /* primes discovered so far       */
static size_t   primes_cnt  = 0;
static size_t   primes_cap  = 0;

static uint64_t *cum_sum64  = NULL;  /* cumulative sums of primes     */
static size_t   cum_cnt     = 0;
static size_t   cum_cap     = 0;

/* Build primes and their cumulative sums until the total sum reaches
   at least `target`.  The sum is stored as uint64_t, independent of
   the seq_t type. */
static void
ensure_primes_upto(uint64_t target)
{
    if (!cum_sum64) {
        primes_cap = 100;
        cum_cap    = 100;
        primes     = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        cum_sum64  = (uint64_t *)malloc(cum_cap * sizeof(uint64_t));
        if (!primes || !cum_sum64) return;  /* silently fail on OOM */
        primes_cnt = cum_cnt = 0;
    }

    while (cum_cnt == 0 || cum_sum64[cum_cnt - 1] < target) {
        /* find next prime */
        uint64_t cand = primes_cnt ? primes[primes_cnt - 1] + 1 : 2;
        while (!is_prime(cand)) ++cand;

        /* grow primes array if required */
        if (primes_cnt == primes_cap) {
            size_t new_cap = primes_cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
            if (!tmp) return;
            primes = tmp;
            primes_cap = new_cap;
        }
        primes[primes_cnt++] = cand;

        /* compute new cumulative sum */
        uint64_t new_sum = (cum_cnt ? cum_sum64[cum_cnt - 1] : 0) + cand;

        /* grow cumulative sum array if required */
        if (cum_cnt == cum_cap) {
            size_t new_cap = cum_cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(cum_sum64, new_cap * sizeof(uint64_t));
            if (!tmp) return;
            cum_sum64 = tmp;
            cum_cap   = new_cap;
        }
        cum_sum64[cum_cnt++] = new_sum;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* undefined for n < 1 */

    uint64_t target = (uint64_t)n;
    ensure_primes_upto(target);

    /* binary search for first cumulative sum ≥ n */
    size_t lo = 0, hi = cum_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (cum_sum64[mid] < target)
            lo = mid + 1;
        else
            hi = mid;
    }

    /* lo is the 0‑based run index: 0 → ones, 1 → zeros, etc. */
    return (lo % 2 == 0) ? (seq_t)1 : (seq_t)0;
}
