/*
 * A227138  Positive solutions of the Pell equation x^2 - 89*y^2 = -1.
 *          Solutions y = 53*a(n).
 * https://oeis.org/A227138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1000002*a(n-1) - a(n-2) (n >= 2) Initial values: a(0) =
 *            1, a(1) = 1000001 This recurrence generates the sequence of
 *            positive solutions y = 53*a(n) of the Pell equation x^2 - 89*y^2
 *            = -1
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cached sequence values */
static size_t  seq_cap   = 0;     /* allocated capacity of seq_cache */
static idx_t   seq_len   = 0;     /* number of values computed so far */

seq_t a(idx_t n)
{
    /* initialisation on first call */
    if (!seq_cache) {
        seq_cap = 2;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_cache) abort();          /* allocation failed */
        seq_cache[0] = 1;
        seq_cache[1] = (seq_t)1000001;
        seq_len = 2;
    }

    /* return already computed value */
    if (n < (idx_t)seq_len)
        return seq_cache[(size_t)n];

    /* extend cache to at least n+1 elements */
    if ((size_t)(n + 1) > seq_cap) {
        size_t new_cap = seq_cap;
        while (new_cap <= (size_t)(n + 1))
            new_cap *= 2;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!seq_cache) abort();          /* allocation failed */
        seq_cap = new_cap;
    }

    /* compute missing values iteratively */
    for (size_t i = seq_len; i <= (size_t)n; ++i) {
        seq_cache[i] = (seq_t)1000002 * seq_cache[i - 1] - seq_cache[i - 2];
    }
    seq_len = (idx_t)(n + 1);

    return seq_cache[(size_t)n];
}
