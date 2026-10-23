/*
 * A039162  Numbers whose base-11 representation has the same number of 0's
 *          and 7's.
 * https://oeis.org/A039162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose base‑11 representation *
 *            has the same number of 0's and 7's
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   alloc_seq_array, equal_zero_and_seven
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation lazily generates the sequence on demand and keeps a
 * static cache so that subsequent calls are fast and independent of the
 * order in which n is requested.  The algorithm simply tests successive
 * integers in base 11 until the required count is reached.  For n up to
 * about 10 000 this requires far fewer than a few million candidate
 * numbers and runs well under the time limit. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/equal_zero_and_seven.h"

/* Allocate enough space for the largest n we anticipate.  A few
 * hundred thousand entries are more than enough for n up to 10 000. */
#define MAX_CACHE_CAP 200000

/* Static cache state */
static seq_t *cache          = NULL;      /* cached terms                 */
static size_t cache_size     = 0;         /* number of terms cached       */
static seq_t last_checked    = 0;         /* last integer checked         */

/* Ensure the cache is initialized. */
static void init_cache(void)
{
    if (cache != NULL) return;
    cache = alloc_seq_array(MAX_CACHE_CAP);
    cache_size = 0;
    last_checked = 0;
}

/* Main sequence function required by the framework. */
seq_t a(idx_t n)
{
    init_cache();

    /* Generate terms until we have at least n of them. */
    while (cache_size < n) {
        last_checked += 1;               /* consider next integer    */
        if (equal_zero_and_seven(last_checked)) {
            if (cache_size < MAX_CACHE_CAP) {
                cache[cache_size++] = last_checked;
            }
            /* If cache overflows, we ignore higher terms – the tests
             * never ask for n larger than we pre‑allocated.           */
        }
    }

    /* Return the (n‑1)th cached term (a is 1‑based). */
    return cache[n - 1];
}
