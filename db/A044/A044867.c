/*
 * A044867  Positive integers having the same number of base-11 runs of odd
 *          length as even.
 * https://oeis.org/A044867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has_equal_odd_even_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044867: Positive integers having the same number of base-11 runs of odd length as even.
   The code computes terms by scanning positive integers and keeping all found terms in a static
   table.  It returns the n-th term (1‑based index).  The implementation works for any
   build where seq_t supports the usual arithmetic and comparison operators. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_equal_odd_even_runs.h"

seq_t a(idx_t n)
{
    static seq_t *table = NULL;      /* table of found terms */
    static size_t table_len = 0;     /* number of terms stored */
    static size_t table_cap = 0;     /* allocated capacity */
    static seq_t next_start = 1;     /* next integer to test */

    /* initialize static storage */
    if (table_cap == 0) {
        table_cap = 64;
        table = (seq_t *)malloc(table_cap * sizeof(seq_t));
    }

    /* extend table until it contains at least n terms */
    while (table_len < (size_t)n) {
        seq_t candidate = next_start;
        /* find next qualifying number */
        while (!has_equal_odd_even_runs(candidate)) {
            candidate++;
        }
        /* store it */
        if (table_len == table_cap) {
            table_cap <<= 1;
            table = (seq_t *)realloc(table, table_cap * sizeof(seq_t));
        }
        table[table_len++] = candidate;
        next_start = candidate + 1;
    }

    return table[n - 1];
}
