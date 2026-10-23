/*
 * A182108  Odd composite numbers in successive intervals [2^i + 1 .. 2^(i+1)
 *          - 1], i=1,2,3,... such that there are only composite symmetric XOR
 *          couples in either the original interval or any recursively halved
 *          interval that contains them.
 * https://oeis.org/A182108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computed by scanning odd composites and keeping only those *
 *            that, in their interval [2^i+1 .. 2^(i+1)-1] and in every
 *            recursively halved * sub‑interval containing them, have an odd
 *            composite symmetric XOR partner. * The function memoises results
 *            for repeated calls
 * Range:     u8 none, u16 n <= 968, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Utility functions supplied by the build system */
#include "../utils/is_prime.h"
#include "../utils/floor_log2_uint64.h"

/* Check whether an odd composite n satisfies the XOR partner condition
 * in its interval and all recursively halved intervals that contain n. */
static bool valid_candidate(idx_t n)
{
    /* Locate the interval [L,R] that contains n */
    unsigned i = floor_log2_uint64(n - 1);
    idx_t L = ((idx_t)1 << i) + 1;
    idx_t R = ((idx_t)1 << (i + 1)) - 1;

    while (L < R) {
        /* Current sub‑interval length */
        idx_t len = R - L + 1;
        /* Offset of n from the left end */
        idx_t offset = n - L;
        /* Compute the symmetric partner at this level */
        idx_t partner = (len & 1) ? (R - offset) : (R - 1 - offset);

        /* Partner must be odd and composite */
        if ((partner & 1) == 0 || is_prime(partner))
            return false;

        /* Descend into the sub‑interval that still contains n */
        idx_t mid = (L + R) >> 1;
        if (n <= mid)
            R = mid;
        else
            L = mid + 1;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Memoisation buffer */
    static seq_t *cache     = NULL;
    static size_t   cap     = 0;
    static idx_t    len     = 0;
    static idx_t    cur     = 3;      /* smallest odd composite to test */

    while (len < n) {
        /* Find next odd composite candidate */
        while (true) {
            if ((cur & 1) == 0) { /* even */
                ++cur;
                continue;
            }
            if (is_prime(cur)) {   /* skip primes */
                ++cur;
                continue;
            }
            break;                 /* cur is odd composite */
        }

        if (valid_candidate(cur)) {
            /* Grow cache if needed */
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 64 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { free(cache); cache = NULL; cap = 0; len = 0; return 0; }
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = (seq_t)cur;
        }
        ++cur;
    }
    return cache[n - 1];
}
