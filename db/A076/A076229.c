/*
 * A076229  Smallest number such that A076228(a(n)) = n.
 * https://oeis.org/A076229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Min{x : A076228(x) = n}
 * Range:     u8 n <= 11, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   count_less_than
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence of minimal starting numbers whose Collatz
   trajectory contains exactly n terms strictly smaller than the
   starting number.  This implementation uses incremental search
   with memoisation.  It is fully compliant with C11 and C++17. */

#include <stdlib.h> /* malloc, realloc, abort */
#include "../utils/count_less_than.h"

/* Memoised table: min_x[k] = minimal x with A076228(x) = k.
   Index 0 is unused; a value of 0 indicates 'unset'. */
static seq_t *min_x = NULL;
static size_t min_x_len = 0;   /* current capacity (size of the array)   */
static idx_t last_x = 1;       /* largest x that has already been processed */

/* Public function */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, but avoid UB */

    /* Search until a(n) has been found. */
    for (;;) {
        /* Stop if we already have a value for this n. */
        if (min_x_len > (size_t)n && min_x[(size_t)n] != 0)
            break;

        /* Generate next candidate. */
        ++last_x;
        seq_t val = last_x;
        seq_t cnt = count_less_than(val);

        /* Ignore count 0 (occurs only for val = 1). */
        if (cnt == 0)
            continue;

        /* Grow the table if the computed count is outside current bounds. */
        if ((size_t)cnt + 1 > min_x_len) {
            size_t new_len = (size_t)cnt + 1;   /* indices 0..cnt inclusive */
            seq_t *tmp = (seq_t *)realloc(min_x, new_len * sizeof(seq_t));
            if (!tmp) abort();                 /* Out of memory */
            /* Initialise new entries to 0 */
            for (size_t i = min_x_len; i < new_len; ++i)
                tmp[i] = 0;
            min_x = tmp;
            min_x_len = new_len;
        }

        /* Record minimal value for this count if not already set. */
        if (min_x[(size_t)cnt] == 0)
            min_x[(size_t)cnt] = val;
    }

    return min_x[(size_t)n];
}
