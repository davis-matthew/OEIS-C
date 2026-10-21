/*
 * A230104  Numbers k such that m + (product of digits of m) is never equal to
 *          k.
 * https://oeis.org/A230104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: build a sieve for m up to a fixed limit.  
   For each m compute m + product_of_digits(m) and mark that number
   as reachable.  After the sieve is built, the array `missing` holds
   all integers that are *not* reachable and the function a(n) simply
   returns the (n-1)-th element of that array.  The limits are
   chosen large enough to cover the first 10,000 terms of the sequence.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_product.h"

static bool init_done = false;
static seq_t *missing = NULL;
static size_t missing_len = 0;

static void build_sieve(void)
{
    if (init_done) return;
    init_done = true;

    /* Limits are chosen to safely cover at least the first 10,000 terms. */
    const idx_t MAX_M = 200000;          /* iterate m up to this value   */
    const size_t MAX_VISIT = 1000000;    /* size of visited array        */

    /* Allocate visited array and zero it. */
    bool *visited = (bool *)malloc(MAX_VISIT * sizeof(bool));
    if (!visited) return;            /* allocation failure:  ignore, relies on caller to catch it */
    memset(visited, 0, MAX_VISIT * sizeof(bool));

    /* Mark all reachable numbers. */
    for (idx_t m = 1; m <= MAX_M; ++m) {
        seq_t prod = digit_product((seq_t)m);
        seq_t k = (seq_t)m + prod;
        if (k < (seq_t)MAX_VISIT) visited[(size_t)k] = true;
    }

    /* Prepare missing array.  Size MAX_VISIT is more than enough to
       hold all missing numbers up to the threshold. */
    missing = (seq_t *)malloc(MAX_VISIT * sizeof(seq_t));
    missing_len = 0;

    for (size_t k = 1; k < MAX_VISIT; ++k) {
        if (!visited[k]) {
            missing[missing_len++] = (seq_t)k;
        }
    }

    free(visited);
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    build_sieve();
    if (n == 0) return 0;      /* but the sequence is 1‑based */
    return missing[n - 1];
}
