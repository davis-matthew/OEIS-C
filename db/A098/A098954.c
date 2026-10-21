/*
 * A098954  Slowest increasing sequence where each number is such that at
 *          least one pair of adjacent digits are consecutive, the second one
 *          being the smallest.
 * https://oeis.org/A098954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   has_desc_adj_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098954:
 * a(n) is the n‑th natural number (n≥1) that contains at least one pair
 * of adjacent digits that are consecutive with the larger digit first
 * (i.e. a descending consecutive pair "xy" with x=y+1).
 * The function scans the integers starting from 10 and collects those that
 * satisfy the property. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_desc_adj_idx.h"

/* Main sequence accessor.
 * N is 1‑based; a(1) = 10, a(2) = 21, etc. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n < 1 */

    /* Cache the generated terms to allow non‑monotonic calls. */
    static seq_t cache[200000];   /* large enough for the tests */
    static idx_t size     = 0;   /* number of terms stored so far */
    static idx_t cand     = 10;  /* current integer to test */

    /* Generate terms until we have at least n of them. */
    while (size < n) {
        /* Advance cand until it satisfies the property. */
        while (!has_desc_adj_idx(cand))
            cand++;

        /* Store the term (truncated to seq_t if necessary). */
        cache[size++] = (seq_t)cand;
        cand++;                     /* next candidate to test */
    }

    return cache[n - 1];
}
