/*
 * A146972  Twin primes in A046704.
 * https://oeis.org/A146972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   append_candidate, digit_sum, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A146972: twin primes in A046704.
 * The sequence lists, in ascending order, every number that occurs as
 * a member of a twin prime pair (p,p+2) whose both members are prime
 * and whose digit sums are also prime.  The first 24 terms are
 * 3,5,7,41,43,… as given in the OEIS entry.
 *
 * The routine does not use a hard‑coded table; it generates terms
 * on demand and memoises them so that calls with arbitrary indices
 * return the correct value.  No external state beyond static
 * variables in this file is required. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/digit_sum.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n) {
    /* Static memoisation state */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static size_t cache_sz  = 0;
    static uint64_t cur     = 3;          /* next odd candidate to test */
    static seq_t last_added = 0;           /* last term added to cache */

    if (n == 0) return 0; /* undefined for offset 1, guard */

    /* Produce terms until we have at least n of them */
    while (cache_sz < (size_t)n) {
        /* Scan the odd numbers until we find the next qualifying twin pair */
        while (true) {
            if (is_prime((idx_t)cur) && is_prime((idx_t)(cur + 2))) {
                seq_t ds1 = digit_sum((seq_t)cur);
                seq_t ds2 = digit_sum((seq_t)(cur + 2));
                if (is_prime((idx_t)ds1) && is_prime((idx_t)ds2)) {
                    /* Add the smallest member only if it hasn't already been added */
                    if ((seq_t)cur > last_added) {
                        append_candidate(&cache, &cache_sz, &cache_cap, (seq_t)cur);
                        last_added = (seq_t)cur;
                    }
                    /* Always add the larger member – it is new because it is > cur. */
                    append_candidate(&cache, &cache_sz, &cache_cap, (seq_t)(cur + 2));
                    last_added = (seq_t)(cur + 2);

                    cur += 2;   /* move to the next odd candidate for future searches */
                    break;      /* proceed to fill cache until size reaches n */
                }
            }
            cur += 2;          /* skip to the next odd number */
        }
    }

    return cache[n - 1];
}
