/*
 * A189641  Partial sums of A189640.
 * https://oeis.org/A189641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(floor((n+2)/3)) + floor(n/3), a(1)=0
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A189641: Partial sums of A189640 */
#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t seq_cache_max = 0;
static size_t seq_cache_cap = 0;

seq_t a(idx_t n) {
    /* Initialize the cache on first call */
    if (seq_cache == NULL) {
        seq_cache_cap = 2;                       /* indices 0 and 1 */
        seq_cache = (seq_t*)malloc(seq_cache_cap * sizeof(seq_t));
        if (seq_cache == NULL) return 0;         /* allocation failure */
        seq_cache[0] = 0;                        /* unused */
        seq_cache[1] = 0;                        /* a(1) = 0 */
        seq_cache_max = 1;
    }

    /* Grow the cache if the requested index is beyond current capacity */
    if ((idx_t)seq_cache_cap <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;               /* allocation failure */
        seq_cache = tmp;
        seq_cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    if (n > seq_cache_max) {
        for (idx_t i = seq_cache_max + 1; i <= n; ++i) {
            seq_t term = (seq_t)(i / 3);          /* floor(i/3) */
            idx_t dep = (i + 2) / 3;              /* floor((i+2)/3) */
            term += seq_cache[dep];
            seq_cache[i] = term;
        }
        seq_cache_max = n;
    }

    return seq_cache[n];
}
