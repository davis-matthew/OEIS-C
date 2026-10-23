/*
 * A043586  Numbers whose base-3 representation has exactly 6 runs.
 * https://oeis.org/A043586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth integer whose base‑3 representation has exactly 6 runs.
 * The function does this by brute‑force scanning of integers in increasing
 * order, caching the results so that repeated calls are fast.  The cache
 * grows only up to the largest requested index, which is sufficient for
 * all n <= 10000. */
#include <stddef.h>   /* for size_t */

/* Cache of the first numbers found.  The size 20000 is ample for n <= 10000
 * and keeps the implementation simple without using dynamic allocation,
 * which would not work for arbitrary‑precision seq_t types. */
static seq_t cache[20000];
static size_t cache_len = 0;   /* number of cached terms */
static idx_t next_check = 0;   /* next integer to examine */
static idx_t limit = 59049;    /* current bound, doubled if exhausted */

/* Return the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    /* Zero or negative index is undefined; we simply return 0. */
    if (n == 0) return 0;

    /* If the cache already contains the answer, return it. */
    if (cache_len >= (size_t)n) return cache[n - 1];

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Expand the search window if necessary. */
        if (next_check > limit) {
            limit *= 2;
        }

        /* Count runs in the base‑3 representation of next_check. */
        int runs = 0;
        int prev = -1;
        idx_t x = next_check;
        if (x == 0) {
            runs = 1;  /* base‑3 "0" has one run */
        } else {
            while (x > 0) {
                int digit = (int)(x % 3);
                if (digit != prev) {
                    ++runs;
                    prev = digit;
                }
                x /= 3;
            }
        }

        /* If we found a number with exactly 6 runs, cache it. */
        if (runs == 6) {
            cache[cache_len++] = (seq_t)next_check;
        }

        ++next_check;
    }

    return cache[n - 1];
}
