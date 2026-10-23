/*
 * A137255  a(n) = 2*a(n-1) + 4*a(n-2) - 6*a(n-3) - 3*a(n-4) for n > 3, with
 *          a(0)=1, a(1)=2, a(2)=4, a(3)=8.
 * https://oeis.org/A137255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 102,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1)+4*a(n-2)-6*a(n-3)-3*a(n-4);
   Initial values: a(0)=1, a(1)=2, a(2)=4, a(3)=8 */
#include <stddef.h>   /* size_t */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Compute the nth term of sequence A137255. */
seq_t a(idx_t n)
{
    /* Base values for n < 4 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)8;

    /* static cache to memoise computed values */
    static seq_t *cache = NULL;      /* pointer to array of seq_t */
    static size_t cap = 0;           /* current allocated size */
    static idx_t last_computed = 3;  /* highest index with a computed value */

    /* Ensure cache is large enough and initial values are set */
    if (cap <= n) {
        size_t new_cap = (n + 1) * 2;   /* double to reduce reallocs */
        if (new_cap < 4) new_cap = 4;
        cache = seq_realloc(cache, cap, new_cap);
        cap = new_cap;
        /* Initialise base values if this is the first allocation or if they were
           not yet written (e.g., after a realloc that pointed to new memory).  */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)8;
    }

    /* Compute any missing terms up to n */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            cache[i] = cache[i-1] * ((seq_t)2)
                     + cache[i-2] * ((seq_t)4)
                     - cache[i-3] * ((seq_t)6)
                     - cache[i-4] * ((seq_t)3);
        }
        last_computed = n;
    }

    return cache[n];
}
