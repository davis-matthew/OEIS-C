/*
 * A218729  a(n) = (26^n - 1)/25.
 * https://oeis.org/A218729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (26^n - 1)/25 Recurrence: a(0) = 0, a(1) = 1, a(n) =
 *            27*a(n-1) - 26*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;   /* memoized values */
static size_t cap   = 0;      /* number of elements currently cached */

/* Compute a(n) = (26^n - 1)/25 for any n >= 0 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* If we need more space, allocate a new array and copy old values */
    if (cache == NULL || (size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;                      /* need n+1 slots */
        seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);                           /* allocation failed */
        if (cache != NULL) {
            memcpy(tmp, cache, cap * sizeof(seq_t));        /* copy what we have */
            free(cache);                                    /* drop old block */
        }

        /* Initialize base values if they were not pre‑filled */
        if (cap == 0) {
            tmp[0] = 0;
            tmp[1] = 1;
            cap   = 2;
        }

        /* Generate missing terms up to n using the recurrence */
        for (size_t i = cap; i < new_cap; ++i) {
            tmp[i] = 27 * tmp[i - 1] - 26 * tmp[i - 2];
        }

        cache = tmp;
        cap   = new_cap;
    }

    return cache[n];
}
