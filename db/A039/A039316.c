/*
 * A039316  Numbers whose base-7 representation has the same nonzero number of
 *          1's and 4's.
 * https://oeis.org/A039316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th positive integer whose base‑7 representation
 *            contains the same non‑zero number of 1's and 4's
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

/* Cache for memoisation. */
static seq_t *cache      = NULL;
static idx_t cacheCount  = 0;   /* number of terms stored */
static size_t cacheCap   = 0;   /* capacity of the allocation */

/* Test whether x has equal non‑zero counts of 1 and 4 in base‑7. */
static bool is_good(seq_t x)
{
    unsigned c1 = 0, c4 = 0;
    seq_t t = x;
    while (t != 0) {
        seq_t d = t % 7;
        if (d == 1)      c1++;
        else if (d == 4) c4++;
        t /= 7;
    }
    return (c1 == c4) && (c1 > 0);
}

/* Return the n‑th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not used – we only index from 1 */

    /* Grow cache until it contains at least n elements. */
    while (cacheCount < n) {
        seq_t candidate = (cacheCount == 0) ? 1 : cache[cacheCount - 1] + 1;
        while (!is_good(candidate)) {
            ++candidate;
        }

        /* Resize the cache if required. */
        if (cacheCount >= cacheCap) {
            size_t newCap = (cacheCap == 0) ? 16 : cacheCap * 2;
            seq_t *tmp = seq_realloc(cache, cacheCap, newCap);
            if (!tmp) {
                /* Fatal error: allocation failed. */
                exit(EXIT_FAILURE);
            }
            cache    = tmp;
            cacheCap = newCap;
        }

        cache[cacheCount++] = candidate;
    }

    return cache[n - 1];   /* offset 1: a(1) is the first stored element */
}
