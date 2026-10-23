/*
 * A154778  Numbers of the form a^2 + 5b^2 with positive integers a,b.
 * https://oeis.org/A154778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) computed by searching for the next integer expressible as a^2 + 5b^2
   with positive integers a,b. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

static seq_t *cache = NULL;        /* stored sequence values */
static size_t cached_len = 0;      /* number of values stored */
static size_t cache_cap = 0;       /* allocated capacity */
static idx_t last_checked = 5;     /* last integer examined (initially 5,
                                     so the first candidate is 6) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* undefined for n < 1, but return 0 safely */

    if (!cache) {                   /* first call: allocate initial cache */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cached_len = 0;
        last_checked = 5;
    }

    /* generate terms until we have at least n of them */
    while (cached_len < (size_t)n) {
        for (idx_t x = last_checked + 1; ; ++x) {
            seq_t xx = (seq_t)x;
            idx_t max_b = (idx_t)floor_sqrt(xx / (seq_t)5);
            bool found = false;

            for (idx_t b = 1; b <= max_b; ++b) {
                seq_t t = (seq_t)5 * (seq_t)b * (seq_t)b;
                seq_t a2 = xx - t;
                if (a2 <= 0)
                    continue;
                seq_t a_val = floor_sqrt(a2);
                if ((a_val * a_val == a2) && (a_val > 0)) {
                    found = true;
                    break;
                }
            }

            if (found) {
                /* grow cache if needed */
                if (cached_len >= cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 16;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cached_len++] = xx;
                last_checked = x;
                break;   /* proceed to next term */
            }
        }
    }

    return cache[n - 1];
}
