/*
 * A077222  a(1) = 1 and then alternately the smallest even and odd numbers
 *          not occurring earlier such that the sum of two successive terms is
 *          a square.
 * https://oeis.org/A077222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   floor_sqrt, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A077222: a(0)=1; for n>0, a(n) is the smallest even (odd) number not
   appearing earlier such that a(n)+a(n-1) is a perfect square, with the
   parity alternating starting with even at n=1. The code searches
   for the minimal candidate each step. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/grow_seq_array.h"

static seq_t *cache = NULL;       /* sequence cache */
static idx_t cache_len = 0;       /* number of terms stored */
static idx_t cache_cap = 0;       /* allocated capacity */

seq_t a(idx_t n)
{
    /* initialize cache if empty */
    if (cache_len == 0) {
        /* allocate space for the first term */
        grow_seq_array(&cache, &cache_cap, 1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* compute terms up to n if necessary */
    while (cache_len <= n) {
        idx_t idx = cache_len;               /* index of term to compute */
        bool need_even = (idx & 1);          /* odd index → even needed */

        /* start with the smallest integer S such that S^2 > a(idx-1) */
        seq_t S = floor_sqrt(cache[idx - 1]) + (seq_t)1;
        seq_t cand;
        bool found = false;

        while (!found) {
            /* candidate = S^2 - a(idx-1) */
            cand = S * S - cache[idx - 1];

            /* correct parity? */
            if (((cand & (seq_t)1) == 0) == need_even) {
                /* check if cand has appeared before */
                bool used = false;
                for (idx_t j = 0; j < cache_len; ++j) {
                    if (cache[j] == cand) {
                        used = true;
                        break;
                    }
                }
                if (!used) {
                    found = true;
                    break;
                }
            }
            ++S;     /* try next square root */
        }

        /* ensure capacity for the new term */
        if (cache_len + 1 > cache_cap) {
            grow_seq_array(&cache, &cache_cap, cache_len + 1);
        }

        cache[cache_len] = cand;
        ++cache_len;
    }

    return cache[n];
}
