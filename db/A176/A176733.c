/*
 * A176733  a(n) = (n+6)*a(n-1) + (n-1)*a(n-2), a(-1)=0, a(0)=1.
 * https://oeis.org/A176733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+6)*a(n-1) + (n-1)*a(n-2), a(-1)=0, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 369 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
/* allocate and reallocate arrays of seq_t, works for both plain integer
   and arbitrary‑precision integer types */

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *seq = NULL;
    static idx_t cache_size = 0;

    /* Ensure the cache can hold indices up to n */
    idx_t prev_size = cache_size;
    if (cache_size < n + 1) {
        idx_t new_size = n + 1;
        seq = seq_realloc(seq, cache_size, new_size);
        cache_size = new_size;
    }

    /* Compute missing terms */
    for (idx_t i = prev_size; i <= n; ++i) {
        if (i == 0) {
            seq[0] = (seq_t)1;
        } else if (i == 1) {
            /* (1+6)*a(0) + (1-1)*a(-1) = 7*1 + 0 */
            seq[1] = (seq_t)7;
        } else {
            seq_t t1 = ((seq_t)(i + 6)) * seq[i - 1];
            seq_t t2 = ((seq_t)(i - 1)) * seq[i - 2];
            seq[i] = t1 + t2;
        }
    }

    return seq[n];
}
