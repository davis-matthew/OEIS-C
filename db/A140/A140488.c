/*
 * A140488  Trajectory of 5 under repeated application of the map: n -> n +
 *          second-smallest number that does not divide n.
 * https://oeis.org/A140488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + second-smallest number that does not divide
 *            a(n-1), with a(1) = 5. The map is applied iteratively, selecting
 *            the second smallest positive integer that fails to divide the
 *            current term
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9120 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *cache = NULL;   /* dynamic array holding computed values */
static idx_t cached_size = 0; /* number of terms currently stored */

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for 0, but returned as 0 */

    /* Ensure the cache has room for up to 'n' terms. */
    if (cache == NULL) {
        cache = mem_new(seq_t, n);
        cached_size = 0;
    } else if (cached_size < n) {
        cache = mem_resize(seq_t, cache, n);
        /* cached_size remains unchanged */
    }

    /* Compute missing terms up to 'n'. */
    while (cached_size < n) {
        /* Term 1 is the starting value 5. */
        if (cached_size == 0) {
            cache[0] = 5;
            ++cached_size;
            continue;
        }

        /* Current value for the recurrence (a(k)). */
        seq_t cur = cache[cached_size - 1];

        /* Find the second smallest number that does not divide 'cur'. */
        seq_t add = 0;
        seq_t found = 0;
        for (seq_t k = 2; ; ++k) {
            if (cur % k != 0) {
                ++found;
                if (found == 2) {
                    add = k;
                    break;
                }
            }
        }

        /* Store the next term. */
        cache[cached_size] = cur + add;
        ++cached_size;
    }

    return cache[n - 1];
}
