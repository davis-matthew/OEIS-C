/*
 * A098768  Deficient numbers whose reversal is an abundant number.
 * https://oeis.org/A098768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1188, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, rev_decimal, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Deficient numbers whose decimal reversal is abundant.
   a(n) is the nth integer that is deficient (sum of proper divisors < n)
   and whose decimal reversal is abundant (sum of proper divisors > reversal). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/rev_decimal.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t size = 0;       /* number of stored terms */
    static size_t cap = 0;        /* cache capacity */
    static idx_t next_candidate = 1; /* next integer to test */

    /* Grow cache until we have at least n terms */
    while (size < n) {
        idx_t cand = next_candidate;

        /* Check if cand is deficient */
        seq_t sigma_cand = sigma_of(cand);          /* sum of divisors (incl. cand) */
        seq_t proper_cand = sigma_cand - cand;      /* sum of proper divisors */
        if (proper_cand < cand) {                    /* deficient? */
            /* Compute reversal and test for abundance */
            seq_t rev = rev_decimal((seq_t)cand);
            /* If reversal does not fit in idx_t, skip this candidate */
            if (rev <= (seq_t)UINT64_MAX) {
                idx_t rev_i = (idx_t)rev;
                seq_t sigma_rev = sigma_of(rev_i);
                seq_t proper_rev = sigma_rev - rev_i;
                if (proper_rev > rev_i) {            /* abundant? */
                    /* Append to cache */
                    ensure_capacity(size + 1, &cap, (void **)&cache, sizeof(seq_t));
                    cache[size++] = (seq_t)cand;
                }
            }
        }

        next_candidate = cand + 1;  /* next integer to test */
    }

    return cache[n - 1];
}
