/*
 * A037493  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1.
 * https://oeis.org/A037493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(17*8^n/63); Recurrence for n>=4: a(n) = 8*a(n-1) +
 *            a(n-2) - 8*a(n-3) Initial terms: a(1)=2, a(2)=17, a(3)=138
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* cache[0] holds a(1), cache[1] holds a(2), ... */
static size_t a_cache_len = 0;  /* number of terms cached */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but safe for offset handling */

    size_t idx = (size_t)n;

    /* Extend cache if necessary */
    if (idx > a_cache_len) {
        size_t old_len = a_cache_len;
        size_t new_len = idx;

        seq_t *tmp = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failure: reset cache to safe state */
            free(a_cache);
            a_cache = NULL;
            a_cache_len = 0;
            return 0;
        }
        a_cache = tmp;
        a_cache_len = new_len;

        /* Compute missing terms from old_len+1 up to idx */
        for (size_t i = old_len + 1; i <= idx; ++i) {
            if (i == 1) {
                a_cache[0] = (seq_t)2;
            } else if (i == 2) {
                a_cache[1] = (seq_t)17;
            } else if (i == 3) {
                a_cache[2] = (seq_t)138;
            } else {
                a_cache[i-1] = ((seq_t)8 * a_cache[i-2])          /* 8*a(i-1) */
                              + a_cache[i-3]                       /* + a(i-2) */
                              - ((seq_t)8 * a_cache[i-4]);          /* - 8*a(i-3) */
            }
        }
    }

    return a_cache[idx - 1];
}
