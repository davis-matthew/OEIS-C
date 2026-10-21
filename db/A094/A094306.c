/*
 * A094306  Number of (s(0), s(1), ..., s(n)) such that 0 < s(i) < 6 and |s(i)
 *          - s(i-1)| <= 1 for i = 1,2,...,n, s(0) = 2, s(n) = 4.
 * https://oeis.org/A094306
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A094306: 
 * a(n) = # of sequences s(0), …, s(n) with 0 < s(i) < 6,
 * |s(i) – s(i‑1)| ≤ 1 for 1 ≤ i ≤ n,
 * s(0) = 2, s(n) = 4.  
 * Recurrence (for n > 4):
 *   a(n) = 4*a(n‑1) – 2*a(n‑2) – 4*a(n‑3)
 * Initial values:
 *   a(2) = 1, a(3) = 3, a(4) = 10
 *
 * The function can be called in any order; it memoises already
 * computed values in a static array.
 */

#include <stddef.h>

/* seq_t and idx_t are supplied by the build environment. */
seq_t a(idx_t n)
{
    if (n < 2)          /* below offset: undefined, return 0 safely */
        return (seq_t)0;

    /* static table that grows as needed */
    static seq_t *table = NULL;
    static size_t   size  = 0;

    /* Ensure table has entries up to n */
    if (size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_table = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_table) return (seq_t)0;          /* allocation fail -> 0 */

        /* copy old values */
        for (size_t i = 0; i < size; ++i)
            new_table[i] = table[i];

        /* initialise newly added slots to zero */
        for (size_t i = size; i < new_size; ++i)
            new_table[i] = (seq_t)0;

        free(table);
        table = new_table;
        size  = new_size;
    }

    /* Base cases */
    if (n == 2) {
        table[2] = (seq_t)1;
        return table[2];
    }
    if (n == 3) {
        table[3] = (seq_t)3;
        return table[3];
    }
    if (n == 4) {
        table[4] = (seq_t)10;
        return table[4];
    }

    /* Compute any missing values up to n */
    for (idx_t i = 5; i <= n; ++i) {
        if (table[i] == 0) {          /* not computed yet */
            table[i] = (seq_t)4 * table[i-1]
                      - (seq_t)2 * table[i-2]
                      - (seq_t)4 * table[i-3];
        }
    }

    return table[n];
}
