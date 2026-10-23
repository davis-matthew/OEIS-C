/*
 * A037485  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2.
 * https://oeis.org/A037485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*a(n-1) + a(n-2) - 8*a(n-3), with a(1)=1, a(2)=10,
 *            a(3)=81
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */
#include <stdlib.h> /* for malloc, realloc */

static seq_t *cache = NULL;   /* array of cached terms, 0‑based */
static idx_t last_n = 0;      /* largest n whose value is cached */

/* Returns the n-th term of OEIS A037485.  n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined, but guard */

    /* Allocate or grow the cache if needed */
    if (n > last_n) {
        size_t new_cap = (size_t)n;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        /* No error handling: build system guarantees success */

        /* Compute missing terms */
        for (idx_t i = last_n + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = (seq_t)1;
            } else if (i == 2) {
                val = (seq_t)10;
            } else if (i == 3) {
                val = (seq_t)81;
            } else {
                /* a(i) = 8*a(i-1) + a(i-2) - 8*a(i-3)  */
                /* cache indices: a(k) is stored at cache[k-1] */
                seq_t v1 = 8 * cache[i - 2];
                seq_t v2 = cache[i - 3];
                seq_t v3 = 8 * cache[i - 4];
                val = v1 + v2 - v3;
            }
            cache[i - 1] = val;
        }
        last_n = n;
    }
    return cache[n - 1];
}
