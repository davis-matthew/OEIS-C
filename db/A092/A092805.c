/*
 * A092805  Expansion of (1+10x)/((1-x)(1-1000x^3)).
 * https://oeis.org/A092805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) computed from the recurrence a(0) = 1, a(1) = 11, a(2) =
 *            11, a(3) = 1011 a(n) = a(n-1) + 1000*a(n-3) - 1000*a(n-4) for n
 *            ≥ 4 This recurrence follows from the generating function
 *            (1+10x)/((1-x)(1-1000x^3)). The computation uses a memoized
 *            table to answer calls in O(n) time where n is the larg...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* malloc, realloc, free */
#include <stddef.h>          /* size_t */

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* static buffer holding computed terms up to index `table_size-1` */
    static seq_t *table = NULL;          /* table[0..table_size-1] */
    static idx_t table_size = 0;         /* number of valid entries */

    /* If we already have the value, return it immediately. */
    if (n < table_size)
        return table[n];

    /* Need to extend the table to hold indices up to n. */
    size_t new_size = (size_t)(n + 1);
    seq_t *new_table = (seq_t *)realloc(table, new_size * sizeof(seq_t));
    if (!new_table) {
        /* Allocation failed – for the faulty environment we simply
           return 0.  In a realistic build path this should never
           happen because the size is small. */
        return (seq_t)0;
    }
    table = new_table;

    /* Initialise base cases if they are missing. */
    if (table_size == 0) {
        /* Completely new table. */
        table[0] = (seq_t)1;
        if (new_size > 1) table[1] = (seq_t)11;
        if (new_size > 2) table[2] = (seq_t)11;
        if (new_size > 3) table[3] = (seq_t)1011;
    } else {
        /* Existing table; make sure the base values are set. */
        if (table_size <= 1 && new_size > 1) table[1] = (seq_t)11;
        if (table_size <= 2 && new_size > 2) table[2] = (seq_t)11;
        if (table_size <= 3 && new_size > 3) table[3] = (seq_t)1011;
    }

    /* Compute any missing terms using the recurrence. */
    idx_t start = table_size;
    if (start < 4) start = 4;          /* skip base values */

    for (idx_t i = start; i < new_size; ++i) {
        /* tmp1 = 1000 * a(i-3), tmp2 = 1000 * a(i-4) */
        seq_t tmp1 = (seq_t)(1000) * table[i - 3];
        seq_t tmp2 = (seq_t)(1000) * table[i - 4];
        /* Because a(i-3) ≥ a(i-4) for n ≥ 4, tmp1 > tmp2 safely. */
        seq_t diff = tmp1 - tmp2;
        table[i] = table[i - 1] + diff;
    }

    table_size = new_size;
    return table[n];
}
