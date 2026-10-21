/*
 * A095387  Initial values for 3x+1 trajectories of which the largest term is
 *          9232.
 * https://oeis.org/A095387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 1579, u32 n <= 1579, u64 n <= 1579,
 *            u128 n <= 1579, big any n
 * Helpers:   collatz_max_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A095387 – Initial values for 3x+1 trajectories with maximum 9232.
 * The sequence consists of all integers x such that the maximum
 * value reached along the Collatz (3x+1) trajectory starting from x
 * equals exactly 9232.  The values are listed in increasing order.
 * Formula: a(n) is the nth element of that list. */

#include <stdlib.h>   /* for malloc */
#include "../utils/collatz_max_idx.h"

/* Cache of all qualifying starting values (static for repeated calls). */
static seq_t *cache      = NULL;  /* array of qualifying values           */
static idx_t cache_count = 0;     /* number of values stored in cache     */

/* Build the cache on the first call.  We know there are 1579 elements,
 * so allocate a little more to be safe. */
static void build_cache(void)
{
    if (cache != NULL)          /* already built */
        return;

    const idx_t MAX_VAL = 9232;
    const idx_t CAP     = 2000; /* > 1579 */

    /* Allocate the cache array. */
    cache = (seq_t *)malloc(CAP * sizeof(seq_t));
    if (cache == NULL)          /* allocation failed; abort */
        return;

    /* Scan all integers from 1 to 9232 and store those whose
     * Collatz trajectory maximum equals 9232. */
    for (idx_t i = 1; i <= MAX_VAL; ++i) {
        if (collatz_max_idx(i) == MAX_VAL) {
            cache[cache_count++] = (seq_t)i;
        }
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    build_cache();               /* ensure the cache is ready */

    /* n is 1‑based; return 0 if out of range (unspecified behaviour). */
    if (n < 1 || n > cache_count)
        return 0;

    return cache[n - 1];
}
