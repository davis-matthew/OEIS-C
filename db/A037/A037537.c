/*
 * A037537  Base-4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,1.
 * https://oeis.org/A037537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0 (unused), a(1)=1. For n>1
   a(n)=4*a(n-1)+d where d cycles 1,2,1 with period 3.  */
#include <stdlib.h>

/* Cached values of the sequence. */
static seq_t *cache = NULL;
static size_t cap = 0;          /* allocated size of cache   */
static idx_t max_computed = 0;  /* largest n computed so far */

seq_t a(idx_t n) {
    /* Handle indices below offset – behaviour irrelevant. */
    if (n < 1) {
        return 0;
    }

    /* Allocate more space if needed. */
    if (cache == NULL || cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (new_cache == NULL) {
            /* Allocation failed – return 0.  (Should not happen in tests.) */
            return 0;
        }
        /* Copy existing values. */
        if (cache != NULL && max_computed > 0) {
            for (idx_t i = 0; i <= max_computed; ++i) {
                new_cache[i] = cache[i];
            }
            free(cache);
        }
        cache = new_cache;
        cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    const unsigned char pattern[3] = {1, 2, 1};
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        unsigned char digit = pattern[(i - 1) % 3];
        /* a(i) = 4*a(i-1) + digit */
        cache[i] = cache[i - 1] * (seq_t)4 + (seq_t)digit;
    }
    max_computed = n;

    return cache[n];
}
