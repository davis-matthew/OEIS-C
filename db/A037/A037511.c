/*
 * A037511  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 1,2,0.
 * https://oeis.org/A037511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A037511: Decimal expansion of a(n) is given by the first n terms
    of the periodic sequence with initial period 1,2,0.

    Formula (recurrence):
        a(1) = 1
        a(2) = 12
        a(3) = 120
        a(4) = 1201
        a(n) = 10*a(n-1) + a(n-3) - 10*a(n-4)    for n >= 5
*/

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *seq_cache = NULL;   /* dynamic cache of computed terms */
static size_t cached_size = 0;    /* number of allocated indices (index 0 unused) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; this case is unused */

    size_t idx = (size_t)n;

    /* Allocate or grow cache to hold term a(n) */
    if (cached_size <= idx) {
        seq_t *new_cache = (seq_t*)realloc(seq_cache,
                                           (idx + 1) * sizeof(seq_t));
        if (!new_cache) abort();      /* allocation failure */
        /* Initialize new entries */
        size_t start = (cached_size == 0) ? 1 : cached_size;
        for (size_t i = start; i <= idx; ++i) {
            if (i == 1) {
                new_cache[1] = (seq_t)1;
            } else if (i == 2) {
                new_cache[2] = (seq_t)12;
            } else if (i == 3) {
                new_cache[3] = (seq_t)120;
            } else if (i == 4) {
                new_cache[4] = (seq_t)1201;
            } else { /* recurrence for i >= 5 */
                new_cache[i] =
                    (seq_t)(10 * new_cache[i - 1]
                        + new_cache[i - 3]
                        - 10 * new_cache[i - 4]);
            }
        }
        seq_cache = new_cache;
        cached_size = idx + 1;
    }

    return seq_cache[idx];
}
