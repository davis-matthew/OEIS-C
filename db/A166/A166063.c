/*
 * A166063  23-rough numbers: positive integers that have no prime factors
 *          less than 23.
 * https://oeis.org/A166063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 23‑rough numbers, i.e., positive integers with no prime *
 *            factors less than 23. Sequence indexed from 1
 * Range:     u8 n <= 47, u16 n <= 1200, u32 n <= 1200, u64 n <= 1200,
 *            u128 n <= 1200, big any n
 * Helpers:   is_23_rough
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_23_rough.h"

/* Static cache of computed terms */
static seq_t *seq_cache   = NULL;
static size_t seq_len     = 0;
static size_t seq_cap     = 0;
static seq_t next_candidate = 1;   /* candidate to test next */

/* Ensure the cache can hold at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* Main entry point */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; for n < 1 return 0 */
        return (seq_t)0;

    /* Expand cache until we have at least n terms */
    if ((idx_t)seq_len < n)
    {
        ensure_capacity((size_t)n);
        while ((idx_t)seq_len < n)
        {
            if (is_23_rough(next_candidate))
            {
                seq_cache[seq_len++] = next_candidate;
            }
            ++next_candidate;
        }
    }

    return seq_cache[n-1];
}
