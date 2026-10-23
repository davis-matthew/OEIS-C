/*
 * A016228  Expansion of 1/((1-x)*(1-5*x)(1-6*x)).
 * https://oeis.org/A016228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 30*a(n-2) + 1, a(0) = 1, a(1) = 12
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *seq_cache = NULL;   /* cached sequence values */
static size_t seq_len   = 0;       /* number of values stored */
static size_t seq_cap   = 0;       /* allocated capacity */

/* compute a(n) using the recurrence and memoize results */
seq_t a(idx_t n)
{
    /* trivial base cases */
    if (n == 0) return 1;
    if (n == 1) return 12;

    /* allocate or grow the cache if needed */
    if (seq_cache == NULL) {
        seq_cap = (size_t)(n + 1);
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (seq_cache == NULL) return 0;          /* out of memory */
        seq_cache[0] = 1;
        seq_cache[1] = 12;
        seq_len   = 2;
    } else if (n + 1 > seq_cap) {
        size_t new_cap = seq_cap * 2;
        if (new_cap < (size_t)(n + 1)) new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;                /* out of memory */
        seq_cache = tmp;
        seq_cap   = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = seq_len; i <= (size_t)n; ++i) {
        seq_cache[i] = 11 * seq_cache[i - 1] - 30 * seq_cache[i - 2] + 1;
    }
    seq_len = (size_t)n + 1;

    return seq_cache[n];
}
