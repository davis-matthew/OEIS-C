/*
 * A043430  Numbers having two 2's in base 8.
 * https://oeis.org/A043430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search for integers whose base‑8 representation contains exactly two digit 2's. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously generated terms. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap  = 0;
    /* Next candidate to test (starts at 1). */
    static seq_t cand = 1;

    /* Grow cache until it contains at least n terms. */
    while (cache_size < n)
    {
        /* Ensure we have enough space to store another term. */
        if (cache_size >= cache_cap)
        {
            idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) /* allocation failed */
                return 0;
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Count digit 2's in the octal representation of cand. */
        seq_t tmp = cand;
        unsigned cnt = 0;
        while (tmp != 0)
        {
            if ((tmp % 8) == 2)
                ++cnt;
            if (cnt > 2)          /* early exit */
                break;
            tmp /= 8;
        }
        if (cnt == 2)
            cache[cache_size++] = cand;

        ++cand; /* next candidate to test */
    }

    return cache[(size_t)(n - 1)];
}
