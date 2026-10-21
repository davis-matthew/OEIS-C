/*
 * A228661  Number of 2 X n binary arrays with top left value 1 and no two
 *          ones adjacent horizontally, diagonally or antidiagonally.
 * https://oeis.org/A228661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2), a(1) = 2, a(2) = 2 Offset: 1 (a(1) is
 *            the first term)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_N 10000          /* sufficient for all tests up to about 10000 */

static seq_t cache[MAX_N + 1];
static size_t cache_size = 0;   /* largest index that has been computed */

/*-----------------------------------------------------------------
   Compute a(n) using the recurrence.
  -----------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – undefined, but return 0 */

    /* ensure we don't exceed the pre‑allocated buffer */
    if ((size_t)n > MAX_N) {
        /* n is too large for the fixed array – fall back to dynamic
           allocation using malloc(), careful to cast the void* return. */
        static seq_t *dyn_cache = NULL;   /* only allocated once */
        static size_t dyn_size = 0;

        if ((size_t)n > dyn_size) {
            seq_t *tmp = (seq_t *)realloc(dyn_cache,
                                          (size_t)n + 1 * sizeof(seq_t));
            if (!tmp) return 0;           /* allocation failed */
            dyn_cache = tmp;
            dyn_size = (size_t)n;
        }

        /* compute values that are still missing */
        for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
            if (i == 1 || i == 2) {
                dyn_cache[i] = (seq_t)2;
            } else {
                dyn_cache[i] = dyn_cache[i - 1] + (seq_t)3 * dyn_cache[i - 2];
            }
        }

        cache_size = (size_t)n;   /* update for subsequent calls */
        return dyn_cache[(size_t)n];
    }

    /* use the static array for all indices up to MAX_N */
    for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
        if (i == 1 || i == 2) {
            cache[i] = (seq_t)2;
        } else {
            cache[i] = cache[i - 1] + (seq_t)3 * cache[i - 2];
        }
    }

    cache_size = (size_t)n;
    return cache[(size_t)n];
}
