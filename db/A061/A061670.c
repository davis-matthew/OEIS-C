/*
 * A061670  Distance to nearest prime power p^k, k=0 and k >= 2 (A025475).
 * https://oeis.org/A061670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1490, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = min_{p^k , k=0 or k≥2} |n – p^k|  (k=0 gives 1) */
#include <stdint.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"

static uint64_t *prime_list   = NULL;
static size_t    prime_cnt    = 0;
static idx_t     prime_limit  = 0;   /* largest n for which primes are available */

/* Ensure that prime_list contains all primes ≤ limit (at least 2) */
static void
ensure_primes_upto(idx_t limit)
{
    if (limit < 2) limit = 2;
    if (limit <= prime_limit) return;

    free(prime_list);
    prime_list = NULL;
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;               /* not used for offset 1, but harmless */

    /* Need primes up to at least n (or 2 for n = 1) */
    ensure_primes_upto((n > 1) ? n : 2);

    seq_t best = (seq_t)n - 1;   /* distance to 1 (p^0) */

    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)prime_list[i];
        seq_t val = p * p;          /* p^2 (k starts at 2) */
        seq_t lower_candidate = 0;
        bool  found_lower = false;

        /* iterate powers of p until exceeding n */
        while (val <= (seq_t)n) {
            lower_candidate = val;
            found_lower = true;
            /* proceed to next power; for the 64‑bit setting this
               will never overflow for the n ranges used */
            val *= p;
        }

        /* upper candidate (first power strictly greater than n) */
        seq_t diff_up = val - (seq_t)n;
        if (diff_up < best) best = diff_up;

        /* lower candidate distance */
        if (found_lower) {
            seq_t diff_low = (seq_t)n - lower_candidate;
            if (diff_low < best) best = diff_low;
        }
    }

    return best;
}
