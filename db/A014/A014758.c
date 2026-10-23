/*
 * A014758  Squares of numbers in array formed from odd elements to the right
 *          of middle of rows of triangle of Eulerian numbers that are not 1.
 * https://oeis.org/A014758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014470(n)^2 A014470(n) is the nth odd Eulerian number to
 *            the right of the middle of a row, counting only numbers that are
 *            not 1. a(n) is the square
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, but safe */

    /* Store the computed A014470 values */
    static seq_t *base = NULL;
    static size_t base_len = 0, base_cap = 0;

    /* Current and previous Eulerian rows */
    static seq_t *prev = NULL, *cur = NULL;
    static size_t prev_len = 0;
    static idx_t last_row = 1;

    /* Initialise first Eulerian row: [1] */
    if (!prev)
    {
        prev_len = 1;
        prev = (seq_t *)malloc(prev_len * sizeof(seq_t));
        prev[0] = (seq_t)1;
    }

    /* Generate rows until we have at least n values in base */
    while (base_len < n)
    {
        idx_t next = last_row + 1;         /* size of the new row */
        size_t cur_len = (size_t)next;     /* new row length */

        /* Allocate memory for the new row */
        cur = (seq_t *)realloc(cur, cur_len * sizeof(seq_t));
        if (!cur) return (seq_t)0;         /* allocation failure */

        cur[0] = (seq_t)1;
        cur[cur_len - 1] = (seq_t)1;
        for (size_t k = 1; k + 1 < cur_len; ++k)
        {
            cur[k] = prev[k - 1] * (seq_t)(next - k) +
                     prev[k] * (seq_t)(k + 1);
        }

        /* Collect odd elements strictly to the right of the middle of the row */
        size_t start = (next + 1) / 2;     /* first index right of middle */
        for (size_t k = start; k < cur_len; ++k)
        {
            seq_t v = cur[k];
            if (v != (seq_t)1 && (v & (seq_t)1))
            {
                /* Ensure capacity for base array */
                if (base_cap == 0)
                {
                    base_cap = 4;
                    base = (seq_t *)malloc(base_cap * sizeof(seq_t));
                }
                else if (base_len + 1 > base_cap)
                {
                    size_t new_cap = base_cap * 2;
                    base = (seq_t *)realloc(base, new_cap * sizeof(seq_t));
                    base_cap = new_cap;
                }
                base[base_len++] = v;
            }
        }

        /* Rotate rows: the current row becomes the previous row */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
        prev_len = cur_len - 1;   /* size of the new previous row */

        last_row = next;
    }

    seq_t x = base[n - 1];
    return x * x;
}
