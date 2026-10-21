/*
 * A059418  Triangle T(n,k) arising from enumeration of permutations with
 *          ordered orbits, read by rows (1<=k<=n).
 * https://oeis.org/A059418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) flattens the triangle T(r,k) defined by T(r,1) = r! / 2,
 *            T(r,r) = 1, T(r,k) = (r-2)*T(r-1,k) + T(r-1,k-1) for 1 < k < r.
 *            The sequence a(n) lists the triangle entries row‑by‑row,
 *            starting with the first element of the first row
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state: cumulative list of triangle entries. */
    static seq_t *flat   = NULL;   /* array of all generated entries   */
    static idx_t  flat_len = 0;    /* number of entries stored         */
    static idx_t  flat_cap = 0;    /* allocated capacity in elements   */

    /* State for generating rows incrementally. */
    static seq_t *prev_row = NULL; /* previous row's array            */
    static idx_t  prev_len = 0;    /* length of previous row          */
    static idx_t  cur_row   = 0;    /* number of rows computed so far   */
    static seq_t  fact = 1;        /* factorial of cur_row            */

    if (n == 0)                      /* undefined for n=0, return 0 */
        return 0;

    /* Generate rows until we have at least n entries. */
    while (flat_len < n) {
        /* Next row number (rows are 1‑indexed). */
        ++cur_row;
        idx_t r = cur_row;

        /* Allocate array for the new row. */
        seq_t *row = (seq_t *)malloc(r * sizeof(seq_t));
        if (!row) abort();            /* allocation failure */

        /* Compute the first element: T(r,1) = r! / 2.  For r==1,
           the formula gives 1 by definition. */
        if (r == 1) {
            row[0] = 1;
        } else {
            fact = (seq_t)fact * (seq_t)r;      /* r! = (r-1)! * r   */
            row[0] = fact / (seq_t)2;          /* r! / 2           */
        }

        /* Interior elements: 1 < k < r. */
        for (idx_t j = 1; j + 1 < r; ++j) {    /* j is zero‑based index of k-1 */
            /* T(r,k) = (r-2)*T(r-1,k) + T(r-1,k-1) */
            seq_t term = prev_row[j] * (seq_t)(r - 2) + prev_row[j - 1];
            row[j] = term;
        }

        /* Last element is 1. */
        if (r > 1)
            row[r - 1] = 1;

        /* Append the new row to the flattened list. */
        if (flat_len + r > flat_cap) {
            idx_t new_cap = flat_cap ? flat_cap * 2 : 64;
            while (flat_len + r > new_cap) new_cap <<= 1;
            flat = (seq_t *)realloc(flat, new_cap * sizeof(seq_t));
            if (!flat) abort();
            flat_cap = new_cap;
        }
        for (idx_t i = 0; i < r; ++i)
            flat[flat_len++] = row[i];

        /* Prepare for next iteration: current row becomes previous row. */
        free(prev_row);
        prev_row = row;
        prev_len = r;
    }

    /* The answer is already in the flattened array. */
    return flat[n - 1];
}
