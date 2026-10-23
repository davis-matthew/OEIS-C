/*
 * A136783  Number of multiplex juggling sequences of length n, base state <3>
 *          and hand capacity 3.
 * https://oeis.org/A136783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 27*a(n-2) + 20*a(n-3) with a(1)=1, a(2)=4,
 *            a(3)=20 (OEIS A136783)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#ifdef __cplusplus
#include <new>
#endif

/* Static memoisation structure */
static seq_t **seq_cache      = NULL;   /* array of pointers, 1‑based indexing   */
static size_t seq_cache_cap   = 0;      /* capacity of seq_cache                */
static size_t seq_cache_len   = 0;      /* largest index already computed       */

/* external linkage as required */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;           /* offset 1: undefined for n==0 */

    const size_t idx = (size_t)n;

    /* Expand capacity if necessary */
    if (idx + 1 > seq_cache_cap) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 16;
        while (new_cap <= idx) new_cap <<= 1;   /* ensure new_cap > idx */

        seq_t **tmp = (seq_t**)realloc(seq_cache, new_cap * sizeof(seq_t*));
        assert(tmp);                            /* fatal on OOM */
        seq_cache = tmp;
        seq_cache_cap = new_cap;
    }

    /* Compute missing terms up to idx */
    for (size_t k = seq_cache_len + 1; k <= idx; ++k) {
        seq_t *value = NULL;
#ifdef __cplusplus
        value = new seq_t();          /* construct a seq_t object in C++ */
#else
        value = (seq_t*)malloc(sizeof(seq_t));   /* allocate in C */
#endif
        assert(value);                /* fatal on OOM */

        if (k == 1) {
            *value = (seq_t)1;
        } else if (k == 2) {
            *value = (seq_t)4;
        } else if (k == 3) {
            *value = (seq_t)20;
        } else {
            seq_t term = (seq_t)10 * (*seq_cache[k-1]);
            term -= (seq_t)27 * (*seq_cache[k-2]);
            term += (seq_t)20 * (*seq_cache[k-3]);
            *value = term;
        }

        seq_cache[k] = value;
    }
    seq_cache_len = idx;   /* update largest computed index */

    return *seq_cache[idx];
}
