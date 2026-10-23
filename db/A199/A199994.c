/*
 * A199994  Composite numbers whose multiplicative persistence is 4.
 * https://oeis.org/A199994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth composite number whose multiplicative persistence
 *            equals 4
 * Range:     u8 n <= 2, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   digit_product, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches numbers sequentially, testing each for
 * a multiplicative persistence of 4 and compositeness. Matching numbers
 * are stored in a cache so that multiple unordered calls are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_product.h"   /* seq_t digit_product(seq_t x); */
#include "../utils/is_prime_big.h"    /* bool is_prime_big(seq_t n); */
#include "../utils/grow_seq.h"        /* void grow_seq(seq_t **arr, size_t *cap, size_t need); */

static seq_t *cache = NULL;      /* cached sequence terms */
static idx_t  cache_len = 0;     /* number of cached terms           */
static size_t cache_cap = 0;     /* allocated capacity of cache       */
static idx_t  last_checked = 0;  /* last integer examined            */

/* Return the nth term (1‑based) of sequence A199994. */
seq_t a(idx_t n)
{
    /* N is guaranteed ≥ 1 by caller; guard against accidental 0 */
    if (n == 0) return 0;

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        last_checked++;                                    /* next candidate */

        /* Skip numbers < 2 – they cannot be composite. */
        if (last_checked < 2) continue;

        /* Compute multiplicative persistence. */
        seq_t x = last_checked;
        int steps = 0;
        while (x >= 10) {                                   /* more than one digit */
            if (++steps > 4) break;                         /* too many steps */
            x = digit_product(x);
        }
        if (steps != 4) continue;                          /* persistence ≠ 4 */

        /* Composite? (i.e., not prime). */
        if (is_prime_big(last_checked)) continue;

        /* Store the found term in the cache. */
        if (cache_len == cache_cap)
            grow_seq(&cache, &cache_cap, cache_len + 1);
        cache[cache_len++] = last_checked;
    }

    /* Return the requested term (0‑based indexing). */
    return cache[n - 1];
}
