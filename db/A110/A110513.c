/*
 * A110513  Expansion of (1 + x)/(1 + 2x + x^3).
 * https://oeis.org/A110513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -2*a(n-1) - a(n-3) ; a(0)=1, a(1)=-1, a(2)=2
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 28, i64 n <= 56, i128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static table for memoization */
    static seq_t *table = NULL;
    static idx_t size  = 0;

    /* enlarge table if necessary */
    if (n >= size)
    {
        idx_t new_size = n + 1;
        seq_t *new_tbl = (seq_t *)realloc(table, new_size * sizeof(seq_t));
        if (!new_tbl) return 0;        /* allocation failure – return zero */

        /* compute any missing entries */
        for (idx_t i = size; i < new_size; ++i)
        {
            if (i == 0)
                new_tbl[i] = (seq_t)1;
            else if (i == 1)
                new_tbl[i] = (seq_t)-1;
            else if (i == 2)
                new_tbl[i] = (seq_t)2;
            else
                new_tbl[i] = -2 * new_tbl[i-1] - new_tbl[i-3];
        }

        table = new_tbl;
        size  = new_size;
    }

    return table[n];
}
