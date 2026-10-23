/*
 * A164039  a(n+1) = 3*a(n) - n.
 * https://oeis.org/A164039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; a(n+1) = 3*a(n) - n - 1 */
#include <stdlib.h>
#include <stddef.h>

static seq_t *seq_cache = NULL;
static size_t seq_cap  = 0;   /* allocated capacity */
static size_t seq_len  = 0;   /* number of values stored (seq_len >= 1 after init) */

seq_t a(idx_t n)
{
    /* Initialize cache with a(0) */
    if (seq_cache == NULL) {
        seq_cap = 1;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_cache[0] = 1;      /* a(0) */
        seq_len = 1;
    }

    /* Grow the cache if we need to go beyond the current length */
    if (seq_len <= (size_t)n) {
        size_t new_cap = seq_cap;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        if (new_cap != seq_cap) {
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (tmp) {
                seq_cache = tmp;
                seq_cap = new_cap;
            }
        }

        /* Compute the remaining terms using the recurrence */
        for (idx_t i = seq_len; i <= n; ++i) {
            /* a(i) = 3*a(i-1) - (i-1) - 1 */
            seq_t prev = seq_cache[i - 1];
            seq_t sub  = (seq_t)(i - 1) + 1;
            seq_cache[i] = prev * 3 - sub;
        }
        seq_len = n + 1;
    }

    return seq_cache[n];
}
