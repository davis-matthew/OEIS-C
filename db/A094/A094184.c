/*
 * A094184  Triangle read by rows in which each term equals the entry above
 *          minus the entry left plus twice the entry left-above.
 * https://oeis.org/A094184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 91, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Sequence A094184 – triangle read by rows.
 *
 *  Recurrence:
 *      T(i,0)         = 1
 *      T(i,j)         = T(i-1,j) - T(i,j-1) + 2*T(i-1,j-1)   (j>0)
 *      T(i,j) = 0 for j>i
 *
 *  The overall sequence a(n) is the triangle read row‑by‑row (row 0 first,
 *  row 1 second, …).  offset 0:  a(0) is the first term of the triangle.
 *
 *  Implementation:
 *      * On first call the triangle is built up to the requested index,
 *        storing all terms in a growing cache.
 *      * Subsequent calls use the cache, extending it if necessary.
 *      * Row computation uses only the previous row; no hard‑coded table
 *        of terms is used.
 *
 *  Complexity:  O(n) time and O(n) memory for the largest queried index n.
 */

#include <stdlib.h>   /* malloc, realloc, free, NULL */

seq_t a(idx_t n)
{
    /* static state --------------------------------------------- */
    static seq_t *cache     = NULL;     /* linear storage of terms        */
    static size_t   cached  = 0;        /* number of terms stored         */
    static size_t   cap     = 0;        /* current capacity of cache      */

    static seq_t *prev_row  = NULL;     /* values of previous row         */
    static idx_t  prev_len  = 0;        /* length of previous row         */
    static idx_t  current_row = 0;      /* last fully computed row number */

    /* initialise on first invocation ---------------------------- */
    if (!cache) {
        /* allocate initial cache with some default size */
        cap = 256;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        /* first row: i = 0, only T(0,0) = 1 */
        prev_row = (seq_t *)malloc(sizeof(seq_t));
        prev_row[0] = (seq_t)1;
        prev_len  = 1;
        cache[0] = (seq_t)1;
        cached    = 1;
        current_row = 0;
    }

    /* extend cache until we have at least (n+1) terms --------- */
    while (cached <= n) {
        /* compute next row: i = current_row + 1          */
        idx_t i = current_row + 1;           /* new row number */
        /* allocate array for the new row */
        seq_t *new_row = (seq_t *)malloc((i + 1) * sizeof(seq_t));
        new_row[0] = (seq_t)1;               /* T(i,0) = 1   */

        /* fill columns j = 1 .. i                       */
        for (idx_t j = 1; j <= i; ++j) {
            seq_t up          = (j < prev_len) ? prev_row[j] : (seq_t)0;
            seq_t left_above  = prev_row[j - 1];
            seq_t left        = new_row[j - 1];
            new_row[j] = up - left + (seq_t)2 * left_above;
        }

        /* grow cache if necessary */
        if (cached + (i + 1) > cap) {
            size_t new_cap = cap ? cap * 2 : 256;
            while (cached + (i + 1) > new_cap)
                new_cap *= 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cap   = new_cap;
        }

        /* append new row terms into linear cache */
        for (idx_t j = 0; j <= i; ++j)
            cache[cached++] = new_row[j];

        /* drop old row, keep new row for the next iteration */
        free(prev_row);
        prev_row = new_row;
        prev_len = i + 1;
        current_row = i;
    }

    /* return the requested term. cache[n] is now defined. */
    return cache[n];
}
