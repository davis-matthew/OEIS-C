/*
 * A118543  Start with 1 and repeatedly reverse the digits and add 25 to get
 *          the next term.
 * https://oeis.org/A118543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1, a(n+1) = reverse_decimal(a(n)) + 25
 * Range:     u8 n <= 4, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* offset is 1; n<1 is undefined */
    static seq_t *cache = NULL;                  /* cache of computed terms */
    static size_t len = 0;                       /* number of terms stored */
    static size_t cap = 0;                       /* allocated capacity */

    /* Initial allocation for the first term */
    if (cap == 0) {
        cap = 16;                                /* start with a modest capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;             /* out of memory -> return 0 */
        cache[0] = (seq_t)1;                     /* a(1) = 1 */
        len = 1;
    }

    /* Extend cache up to requested index */
    while (len < (size_t)n) {
        seq_t prev = cache[len - 1];
        seq_t next = reverse_decimal(prev) + (seq_t)25;

        /* Grow capacity if necessary */
        if (len >= cap) {
            cap <<= 1;                           /* double capacity */
            seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;           /* out of memory -> return 0 */
            cache = tmp;
        }

        cache[len++] = next;                     /* store the new term */
    }

    return cache[n - 1];                         /* return 1‑based term */
}
