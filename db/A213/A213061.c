/*
 * A213061  Triangle of Stirling numbers of second kind (A048993) read mod 2.
 * https://oeis.org/A213061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Parity of Stirling numbers of the second kind read from the triangle
 * A213061 (implicit formula: a(n) = S(r,k) mod 2 where
 * r*(r+1)/2 <= n < (r+1)*(r+2)/2 and k = n - r*(r+1)/2). */
 
#include <stdlib.h>
#include <string.h>
#include <math.h>
 
/* Static buffers to hold two successive rows of the triangle. */
static unsigned char *prev_row = NULL;
static unsigned char *cur_row  = NULL;
static size_t row_cap = 0;
 
/* Return the row index r for flat index n.               */
/* The constraint r*(r+1)/2 <= n < (r+1)*(r+2)/2.         */
static idx_t row_index(idx_t n)
{
    if (n == 0)
        return 0;
    /* Rough estimate; then adjust. */
    double d = sqrt((double)8 * (double)n + 1);
    idx_t r = (idx_t)((d - 1.0) / 2.0);
    while (r * (r + 1) / 2 > n) --r;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    return r;
}
 
/* Ensure buffers are large enough for (r+1) entries. */
static void ensure_capacity(idx_t r)
{
    size_t needed = (size_t)(r + 1);
    if (row_cap < needed)
    {
        unsigned char *tmp = (unsigned char*)realloc(prev_row,
                                                     needed * sizeof(unsigned char));
        if (!tmp) exit(EXIT_FAILURE);
        prev_row = tmp;
        tmp = (unsigned char*)realloc(cur_row,
                                      needed * sizeof(unsigned char));
        if (!tmp) exit(EXIT_FAILURE);
        cur_row = tmp;
        row_cap = needed;
    }
}
 
/* Compute a(n). */
seq_t a(idx_t n)
{
    idx_t r = row_index(n);
    ensure_capacity(r);
 
    /* Initialize row 0: S(0,0)=1. */
    prev_row[0] = 1;
 
    /* Build rows 1 .. r. */
    for (idx_t i = 1; i <= r; ++i)
    {
        cur_row[0] = 0;                /* S(i,0)=0 for i>0. */
        for (idx_t k = 1; k <= i; ++k)
        {
            unsigned char term1 = 0;
            if (k <= i - 1 && (k & 1))
                term1 = prev_row[k];
            unsigned char term2 = prev_row[k - 1];
            cur_row[k] = term1 ^ term2; /* Modulo 2 arithmetic. */
        }
        /* Prepare for next iteration: cur -> prev. */
        memcpy(prev_row, cur_row, (size_t)(i + 1));
    }
 
    /* Index within the r-th row. */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;
    return (seq_t)prev_row[k];
}
