/*
 * A118637  Start with 1 and repeatedly reverse the digits and add 30 to get
 *          the next term.
 * https://oeis.org/A118637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = reverse(a(n-1)) + 30, with a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/reverse_digits.h"

/* Return the n-th term of OEIS sequence A118637. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms.  cache[0] holds a(1). */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* Cache capacity */
    static idx_t size  = 0;   /* Number of terms stored */

    if (n < 1)                 /* Offset is 1; below it we return 0 */
        return (seq_t)0;

    /* Initialise cache on first call. */
    if (!cache) {
        cap  = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* Out of memory: abort quietly */
        size = 0;
    }

    /* Ensure the first term is present. */
    if (size == 0) {
        cache[0] = (seq_t)1;  /* a(1) = 1 */
        size = 1;
    }

    /* Grow the cache until it contains the requested term. */
    while (size < n) {
        seq_t last   = cache[size - 1];
        seq_t rev    = reverse_digits(last);
        seq_t next   = rev + (seq_t)30;

        if (size >= cap) {            /* Double capacity */
            cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* Out of memory: abort quietly */
            cache = tmp;
        }

        cache[size] = next;
        size++;
    }

    return cache[n - 1];
}
