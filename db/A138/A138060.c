/*
 * A138060  Triangle read by rows: row 1 = {1}; for n>1, row n is obtained
 *          from row n-1 by applying the morphism 1->1,2; 2->3; 3->4; 4->1.
 * https://oeis.org/A138060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Morphic sequence: rows obtained from the previous row by the morphism
   1 → {1,2}, 2 → {3}, 3 → {4}, 4 → {1}.  The sequence a(n) lists all
   rows consecutively; a(1) is the first element of row 1, a(2) the first
   element of row 2, etc. */

#include <stdlib.h>

static seq_t *seq_arr = NULL;   /* cumulative sequence built so far      */
static idx_t   seq_len = 0;     /* number of stored elements              */
static size_t  seq_cap = 0;     /* current capacity of seq_arr            */

/* Ensure that seq_arr contains at least needed slots. */
static void
ensure_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? (seq_cap << 1) : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_arr = tmp;
    seq_cap = new_cap;
}

/* Information about the most recently generated row. */
static idx_t cur_start = 0;  /* index of its first element in seq_arr */
static idx_t cur_len   = 0;  /* number of elements in that row       */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined index, return 0 for safety */

    /* Initialise on the first call. */
    if (seq_arr == NULL)
    {
        ensure_capacity(1);
        seq_arr[0] = (seq_t)1;
        seq_len = 1;
        cur_start = 0;
        cur_len   = 1;
    }

    /* Generate rows until the cumulative length reaches n. */
    while (seq_len < n)
    {
        /* Compute length of the next row produced from the current row. */
        size_t new_len = 0;
        for (idx_t i = 0; i < cur_len; ++i)
        {
            if (seq_arr[cur_start + i] == (seq_t)1)
                new_len += 2;    /* 1 expands to 2 symbols */
            else
                new_len += 1;    /* 2,3,4 each expand to one symbol */
        }

        /* Ensure enough space for the new row. */
        ensure_capacity((size_t)seq_len + new_len);

        /* Append the expanded row at the end of seq_arr. */
        idx_t pos = seq_len;
        for (idx_t i = 0; i < cur_len; ++i)
        {
            seq_t v = seq_arr[cur_start + i];
            if (v == (seq_t)1)
            {
                seq_arr[pos++] = (seq_t)1;
                seq_arr[pos++] = (seq_t)2;
            }
            else if (v == (seq_t)2)
                seq_arr[pos++] = (seq_t)3;
            else if (v == (seq_t)3)
                seq_arr[pos++] = (seq_t)4;
            else /* v == 4 */
                seq_arr[pos++] = (seq_t)1;
        }

        seq_len += new_len;

        /* The newly generated row becomes the current row. */
        cur_start = seq_len - new_len;
        cur_len   = new_len;
    }

    return seq_arr[n - 1];
}
