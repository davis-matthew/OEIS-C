/*
 * A108146  a(n) = 4*a(n-1) - a(n-2) - a(n-3).
 * https://oeis.org/A108146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2) - a(n-3), a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

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
