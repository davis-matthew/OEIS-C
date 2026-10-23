/*
 * A185762  Number of (n+1) X 3 binary arrays with no 2 X 2 subblock trace
 *          equal to any horizontal or vertical neighbor 2 X 2 subblock trace.
 * https://oeis.org/A185762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   compute_recursively
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 6*a(n-2) + a(n-3)  (n ≥ 4)
   Initial values:
   a(1) = 40
   a(2) = 120
   a(3) = 368 */

#include <stdbool.h>
#include "../utils/compute_recursively.h"

#define MAX_CACHE 10000          /* longest cache we keep */

static seq_t cache[MAX_CACHE + 1];   /* indices 1 … MAX_CACHE */
static idx_t computed_upto = 0;      /* largest index already filled */
static bool initialized = false;     /* cache initialisation flag */

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n = 0 */

    /* initialise cache once */
    if (!initialized) {
        cache[1] = 40;
        cache[2] = 120;
        cache[3] = 368;
        computed_upto = 3;
        initialized = true;
    }

    if (n <= computed_upto)
        return cache[n];          /* already known */

    if (n <= MAX_CACHE) {
        /* extend the cache up to n */
        for (idx_t i = computed_upto + 1; i <= n; ++i)
            cache[i] = cache[i - 1] + (seq_t)6 * cache[i - 2] + cache[i - 3];
        computed_upto = n;
        return cache[n];
    }

    /* n exceeds the cache range – compute on the fly */
    return compute_recursively(n);
}
