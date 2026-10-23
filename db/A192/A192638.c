/*
 * A192638  Numbers n such that 4n + 3 and 16n + 15 are prime.
 * https://oeis.org/A192638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   _is_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A192638: numbers n such that 4*n+3 and 16*n+15 are prime.
   The function finds the nth term by brute force, caching results for
   random access. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/_is_candidate.h"

static seq_t *cache = NULL;     /* cached sequence values               */
static size_t cache_len = 0;    /* number of values currently stored     */
static size_t cache_cap = 0;    /* capacity of the array                 */
static uint64_t last_checked = 0; /* last integer examined               */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0. */
    if (n == 0)
        return 0;

    /* Generate terms until we have at least 'n' of them. */
    while (cache_len < n) {
        ++last_checked;
        if (!_is_candidate(last_checked))
            continue;

        seq_t cand = (seq_t)last_checked;          /* n itself              */
        seq_t p1   = 4 * cand + (seq_t)3;           /* 4*n + 3               */
        seq_t p2   = 16 * cand + (seq_t)15;          /* 16*n + 15             */

        if (is_prime_big(p1) && is_prime_big(p2)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)          /* out of memory – abort gracefully */
                    return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;   /* store the found number */
        }
    }

    return cache[n - 1];                 /* a(1) is stored in cache[0] */
}
