/*
 * A032531  An inventory sequence: triangle read by rows, where T(n, k), 0 <=
 *          k <= n, records the number of k's thus far in the flattened
 *          sequence.
 * https://oeis.org/A032531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   a(n) = value produced by the iterative inventory rule
 * Range:     u8 n <= 2216, u16 n <= 9349, u32 n <= 9349, u64 n <= 9349,
 *            u128 n <= 9349, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inventory triangle read by rows.
   a(n) is the n-th element of the flattened triangle.
   Each value is the current count of the column index, and after
   printing a value we increment the count for that value. */


#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Grow an array of seq_t to at least 'need' elements.
   The array is reallocated (with zeroed new elements) if necessary. */
static void grow_seq_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;

    size_t newcap = (*cap == 0) ? 1 : (*cap << 1);
    while (newcap < need) newcap <<= 1;

#ifdef __cplusplus
    /* Use placement new for C++ builds to construct elements. */
    seq_t *newarr = new seq_t[newcap];
    for (size_t i = 0; i < *cap; ++i) newarr[i] = (*arr)[i];
    delete[] *arr;
    *arr = newarr;
#else
    seq_t *newarr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    if (newarr) {
        /* Zero any newly allocated elements. */
        memset(newarr + *cap, 0, (newcap - *cap) * sizeof(seq_t));
    }
    *arr = newarr;
#endif
    *cap = newcap;
}

seq_t a(idx_t n)
{
    /* static memoization state */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;

    static seq_t *cnt = NULL;
    static size_t cnt_cap = 0;

    /* current position in the triangle */
    static idx_t cur_row = 0;
    static idx_t cur_col = 0;

    /* initial allocation */
    if (seq == NULL) {
        seq_cap = 16;
#ifdef __cplusplus
        seq = new seq_t[seq_cap];
#else
        seq = (seq_t *)calloc(seq_cap, sizeof(seq_t));
#endif
        cnt_cap = 16;
#ifdef __cplusplus
        cnt = new seq_t[cnt_cap];
#else
        cnt = (seq_t *)calloc(cnt_cap, sizeof(seq_t));
#endif
    }

    /* generate terms until index n is available */
    while (seq_len <= (size_t)n) {
        /* Ensure count array can hold cnt[cur_col] and cnt[term] */
        if ((size_t)cur_col >= cnt_cap)
            grow_seq_array(&cnt, &cnt_cap, (size_t)cur_col + 1);

        seq_t term = cnt[cur_col];
        size_t term_idx = (size_t)term;          /* use current value as index */

        if (term_idx >= cnt_cap)
            grow_seq_array(&cnt, &cnt_cap, term_idx + 1);

        /* Ensure the sequence array can hold the new term */
        if (seq_len >= seq_cap)
            grow_seq_array(&seq, &seq_cap, seq_len + 1);

        seq[seq_len++] = term;
        cnt[term_idx]++;                         /* count the new value */

        /* advance to next position in the triangle */
        cur_col++;
        if (cur_col > cur_row) {
            cur_col = 0;
            cur_row++;
        }
    }

    return seq[(size_t)n];
}
