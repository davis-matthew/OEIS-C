/*
 * A108146  a(n) = 4*a(n-1) - a(n-2) - a(n-3).
 * https://oeis.org/A108146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2) - a(n-3), a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *table = NULL;
    static idx_t size = 0;

    if (!table) {
        table = seq_malloc(3);
        table[0] = table[1] = table[2] = (seq_t)1;
        size = 3;
    }

    if (n < size) return table[n];

    /* extend table to hold up to n */
    idx_t newsize = n + 1;
    table = seq_realloc(table, newsize);

    for (idx_t i = size; i <= n; ++i) {
        seq_t term = table[i-1];
        term = term * (seq_t)4;
        term -= table[i-2];
        term -= table[i-3];
        table[i] = term;
    }

    size = newsize;
    return table[n];
}
