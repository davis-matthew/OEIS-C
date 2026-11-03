/*
 * A139269  Twice lucky numbers.
 * https://oeis.org/A139269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * A000959(n)
 * Range:     u8 n <= 27, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper: generate lucky numbers up to 'limit'.
   Returns an array of seq_t lucky numbers (not doubled) and
   the number of elements in *len.  The caller must free the
   returned pointer.  If allocation fails, *output is set to NULL
   and *len is set to 0. */
static void compute_lucky_upto(seq_t limit, seq_t **output, size_t *len)
{
    /* Start with all odd numbers ≤ limit: 1,3,5,... */
    size_t oddcnt = (size_t)(limit / 2 + (limit & 1));
    seq_t *L = mem_new(seq_t, oddcnt);
    if (!L) {
        *output = NULL;
        *len = 0;
        return;
    }

    seq_t val = 1;
    for (size_t i = 0; i < oddcnt; ++i) {
        L[i] = val;
        val += 2;
    }

    size_t sz = oddcnt;
    size_t i = 1;                          /* skip the first element (1) */
    while (i < sz) {
        seq_t step = L[i];
        if (step > (seq_t)sz) break;      /* no more removals can occur */

        /* Build the new list by keeping elements whose (index+1) mod step ≠ 0 */
        size_t write = 0;
        for (size_t j = 0; j < sz; ++j) {
            if ((j + 1) % step != 0) {
                L[write++] = L[j];
            }
        }
        sz = write;
        ++i;
    }

    /* Resize the array to the actual size. */
    seq_t *tmp = mem_resize(seq_t, L, sz);
    if (!tmp && sz > 0) {          /* realloc failed but we still have data */
        mem_free(L);
        *output = NULL;
        *len = 0;
        return;
    }
    *output = tmp;
    *len = sz;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset is 1 */

    /* Cached lucky numbers (not doubled). */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (cache_len >= (size_t)n) {
        return cache[(size_t)n - 1] * (seq_t)2;
    }

    /* Estimate a limit that should contain at least n lucky numbers.
       For n up to 10,000 the n*10 bound is more than sufficient. */
    seq_t limit = (seq_t)n * 10;
    if (limit < 10) limit = 10;          /* small safety guard */

    seq_t *new_cache = NULL;
    size_t new_len = 0;

    /* Try progressively larger limits until we have enough terms. */
    while (1) {
        compute_lucky_upto(limit, &new_cache, &new_len);
        if (!new_cache) return 0;       /* allocation failure */

        if (new_len >= (size_t)n) {
            break;                      /* enough terms gathered */
        }

        /* Not enough: free what we just computed and double the limit. */
        mem_free(new_cache);
        new_cache = NULL;
        new_len = 0;
        limit *= 2;
    }

    /* Replace the old cache (if any). */
    mem_free(cache);
    cache = new_cache;
    cache_len = new_len;

    return cache[(size_t)n - 1] * (seq_t)2;
}
