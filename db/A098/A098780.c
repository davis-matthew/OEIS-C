/*
 * A098780  First n numbers in binary representation concatenated in reverse
 *          order.
 * https://oeis.org/A098780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   For n >= 2,
     a(n) = a(n-1) + n << bitlen_seq_t(a(n-1));
   where bitlen_seq_t(x) returns the number of bits needed to represent x. */

#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* Cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t  computed_max = 0;   /* largest n computed so far */
    static idx_t  capacity   = 0;     /* allocated cache size (index 0 unused) */

    /* Do nothing for n == 0 (offset is 1) */
    if (n == 0)
        return (seq_t)0;

    /* Expand cache if necessary */
    if (n + 1 > capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            exit(EXIT_FAILURE);          /* cannot allocate more memory */
        cache = new_cache;
        capacity = (idx_t)new_cap;
    }

    /* Initialise first term if not yet done */
    if (computed_max < 1) {
        cache[1] = (seq_t)1;
        computed_max = 1;
    }

    /* Compute missing values up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        unsigned bits = bitlen_seq_t(cache[i-1]);   /* length of a(i-1) in bits */
        seq_t term = ((seq_t)i << bits);            /* i shifted by that many bits */
        cache[i] = cache[i - 1] + term;             /* concatenate i before previous string */
    }
    computed_max = n;

    return cache[n];
}
