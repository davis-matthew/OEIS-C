/*
 * A060003  Odd numbers not of the form p + 2*k^2, k>0, p prime.
 * https://oeis.org/A060003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   append_candidate, build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A060003:
   a(n) = the n-th odd integer that cannot be written as p + 2*k^2
   with p prime and k > 0.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/floor_sqrt.h"          /* floor_sqrt(seq_t) */
#include "../utils/append_candidate.h"    /* append_candidate(...) */
#include "../utils/build_primes_upto.h"   /* build_primes_upto(...) */

/* Ensure we have a prime table that contains all primes <= limit. */
static void
ensure_primes_upto(idx_t limit,
                   uint64_t **primes_ptr,
                   size_t     *cnt_ptr,
                   idx_t      *prime_limit)
{
    if (limit <= *prime_limit)
        return;

    idx_t new_limit = (*prime_limit == 0) ? 2 : (*prime_limit << 1);
    while (new_limit < limit)
        new_limit <<= 1;

    uint64_t *tmp   = NULL;
    size_t    tmp_c = 0;
    build_primes_upto(new_limit, &tmp, &tmp_c);

    if (*primes_ptr)
        free(*primes_ptr);

    *primes_ptr   = tmp;
    *cnt_ptr      = tmp_c;
    *prime_limit  = new_limit;
}

seq_t a(idx_t n)
{
    /* Cache of found sequence values. */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* State for generating sequence elements. */
    static seq_t   cand          = 1;          /* current odd candidate */
    static uint64_t *prime_table = NULL;       /* primes up to current limit */
    static size_t    prime_cnt   = 0;
    static idx_t     prime_limit = 0;
    static bool      initialized = false;

    if (!initialized) {
        /* Seed the prime table with at least the prime 2. */
        ensure_primes_upto(2, &prime_table, &prime_cnt, &prime_limit);
        initialized = true;
    }

    while (cache_len < n) {
        /* Make sure we have primes up to the current candidate. */
        idx_t cand_lim = cand > (seq_t)UINT64_MAX
                            ? (idx_t)UINT64_MAX
                            : (idx_t)cand;
        ensure_primes_upto(cand_lim, &prime_table, &prime_cnt, &prime_limit);

        bool representable = false;
        for (size_t i = 0; i < prime_cnt; ++i) {
            seq_t p = (seq_t)prime_table[i];
            if (p > cand - (seq_t)2)
                break;                      /* further primes too large */

            seq_t m = cand - p;
            if ((m & (seq_t)1) != 0)        /* m must be even to be 2*k^2 */
                continue;

            seq_t half = m >> 1;
            seq_t r    = floor_sqrt(half);
            if (r * r == half) {
                representable = true;
                break;
            }
        }

        if (!representable)
            append_candidate(&cache, &cache_len, &cache_cap, cand);

        cand += (seq_t)2;   /* next odd number */
    }

    return cache[n - 1];
}
