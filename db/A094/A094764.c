/*
 * A094764  Trajectory of 7 under repeated application of the map n --> n +
 *          square excess of n.
 * https://oeis.org/A094764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Map: a(0) = 7
   a(n) = a(n‑1) + square_excess(a(n‑1)), where
   square_excess(x) = x – floor_sqrt(x)^2
   (i.e., a(n) = 2*a(n‑1) – floor_sqrt(a(n‑1))^2) */
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    static seq_t *CACHE = NULL;
    static size_t CAP   = 0;
    static idx_t  LAST  = 0;   /* last computed index, 0‑based */

    /* initial setup: a(0) = 7 */
    if (CACHE == NULL) {
        CAP   = 1;
        CACHE = (seq_t *)malloc(CAP * sizeof(seq_t));
        if (CACHE == NULL) return (seq_t)0;   /* allocation failure */
        CACHE[0] = (seq_t)7;
        LAST = 0;
    }

    /* grow the cache if needed */
    if (n >= CAP) {
        size_t newcap = CAP;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(CACHE, newcap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        CACHE = tmp;
        CAP   = newcap;
    }

    /* compute missing terms up to index n */
    for (idx_t i = LAST + 1; i <= n; ++i) {
        seq_t prev = CACHE[i - 1];
        seq_t sq   = floor_sqrt(prev);
        seq_t sq2  = sq * sq;
        CACHE[i]   = prev + (prev - sq2);    /* 2*prev – sq2 */
    }
    LAST = n;

    return CACHE[n];
}
