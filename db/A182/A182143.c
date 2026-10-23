/*
 * A182143  Number of independent vertex sets in the Moebius ladder graph with
 *          2n nodes (n >= 0).
 * https://oeis.org/A182143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + a(n-3) (n ≥ 3) Initial values: a(0) =
 *            1, a(1) = 3, a(2) = 5
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cache for computed values */
static idx_t  seq_cache_len = 0;  /* number of entries currently cached */

/* Return the nth term of OEIS sequence A182143.  */
seq_t a(idx_t n)
{
    /* Ensure the cache is large enough.  */
    if (seq_cache_len == 0) {
        /* Allocate space for the first three terms.  */
        seq_cache_len = 3;
        seq_cache = (seq_t *)malloc(seq_cache_len * sizeof(seq_t));
        if (!seq_cache) return 0;  /* Out of memory; caller should handle.  */
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)3;
        seq_cache[2] = (seq_t)5;
    }

    /* If the requested index is already cached, just return it.  */
    if (n < seq_cache_len) {
        return seq_cache[n];
    }

    /* Grow the cache to accommodate index n.  */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
    if (!tmp) return 0;  /* Out of memory; caller should handle.  */
    seq_cache = tmp;
    seq_cache_len = new_len;

    /* Compute the missing terms using the recurrence.  */
    /* The recurrence holds for i >= 3.  */
    for (idx_t i = 3; i <= n; ++i) {
        seq_cache[i] = seq_cache[i - 1]
                      + (seq_t)3 * seq_cache[i - 2]
                      + seq_cache[i - 3];
    }

    return seq_cache[n];
}
