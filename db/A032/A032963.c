/*
 * A032963  Numbers whose base-13 representation Sum_{i=0..m} d(i)*13^(m-i)
 *          has even d(i) for all odd i.
 * https://oeis.org/A032963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑13 representation has even digits at all odd
 *            positions
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   valid_base13
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/valid_base13.h"

/* Return the n‑th element of sequence A032963 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* offset is 1; behaviour for n==0 is irrelevant */

    /* Static cache of generated terms.  The cache is grown only as needed. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next = 1;    /* the next integer to test */

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < (size_t)n)
    {
        /* Find next valid integer. */
        while (!valid_base13(next))
            ++next;

        /* Grow the cache if necessary. */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();        /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = next;
        ++next;
    }

    return cache[n - 1];
}
