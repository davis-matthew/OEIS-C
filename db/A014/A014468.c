/*
 * A014468  Triangular array formed from elements to right of middle of rows
 *          of the triangle of Eulerian numbers that are greater than 1.
 * https://oeis.org/A014468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A014468:  generate terms by taking each row of the
   Eulerian triangle, selecting the elements strictly to the right
   of the middle of the row and greater than 1, then concatenating
   those elements. */
#include <stdlib.h>

/* static storage for already computed terms */
static seq_t *term_cache = NULL;   /* array of computed terms */
static size_t term_cap   = 0;      /* current capacity of term_cache */
static size_t term_cnt   = 0;      /* number of terms stored */

/* static storage for the previous Eulerian row */
static seq_t *prev_row       = NULL;   /* array holding row of length prev_len */
static size_t prev_len       = 0;      /* length of prev_row (i.e., n of that row) */

/* Append a new term to the cache, reallocating as needed */
static void append_term(seq_t val)
{
    if (term_cnt == term_cap)
    {
        /* grow cache (at least twice the current capacity) */
        size_t new_cap = term_cap == 0 ? 8 : term_cap << 1;
        seq_t *tmp = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return;                     /* allocation failure – abort */
        term_cache = tmp;
        term_cap   = new_cap;
    }
    term_cache[term_cnt++] = val;
}

/* Generate Eulerian rows until we have at least 'need' terms. */
static void generate_until(size_t need)
{
    /* initialise first row if not yet done */
    if (!prev_row)
    {
        prev_row = (seq_t *)malloc(sizeof(seq_t));
        prev_row[0] = (seq_t)1;
        prev_len = 1;                 /* row index n = 1 */
    }

    while (term_cnt < need)
    {
        /* next row has length n = prev_len + 1 */
        idx_t n = (idx_t)(prev_len + 1);
        size_t nlen = (size_t)n;               /* length of the new row */

        /* allocate new row */
        seq_t *new_row = (seq_t *)malloc(nlen * sizeof(seq_t));
        if (!new_row)
            return;                     /* allocation failure – abort */

        /* first and last entries are always 1 */
        new_row[0] = (seq_t)1;
        new_row[nlen - 1] = (seq_t)1;

        /* compute interior entries using recurrence
           A(n,m) = (m+1)*A(n-1,m) + (n-m)*A(n-1,m-1),  1 <= m <= n-2 */
        for (size_t m = 1; m + 1 < nlen; ++m)
        {
            seq_t term1 = (seq_t)(m + 1) * prev_row[m];
            seq_t term2 = (seq_t)(n - m) * prev_row[m - 1];
            new_row[m] = term1 + term2;
        }

        /* middle index (floor((n-1)/2)) */
        size_t mid = (nlen - 1) / 2;

        /* add elements to the right of the middle that are >1 */
        for (size_t m = mid + 1; m < nlen; ++m)
        {
            if (new_row[m] > (seq_t)1)
                append_term(new_row[m]);
        }

        /* free previous row and store the new one for the next step */
        free(prev_row);
        prev_row = new_row;
        prev_len = nlen;
    }
}

/* Main entry for the sequence */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* out of offset range */

    /* generate terms until we have at least 'n' terms stored */
    generate_until((size_t)n);

    return term_cache[(size_t)n - 1];
}
