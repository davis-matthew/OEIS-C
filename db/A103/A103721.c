/*
 * A103721  Column m=4 sequence of triangle A103718(n,m), n>=0, without
 *          leading zeros.
 * https://oeis.org/A103721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |S1(n+5,5)| + |S1(n+5,6)| where S1 are unsigned Stirling
 *            numbers of the first kind
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Dynamically compute the unsigned Stirling numbers of the first kind up to the
   required row.  We store all columns 0..6 of each row, because the recurrence
   for columns 5 and 6 depends on column 4.  The table is expanded only once
   per new maximum n, so repeated calls with arbitrary n are fast. */

static seq_t (*S1)[7] = NULL;   /* pointer to array [row][col] */
static idx_t max_row = 0;       /* highest row that has been computed */
static int initialized = 0;     /* flag whether the table has been initialized */

static void ensure_up_to(idx_t up_to)
{
    if (!initialized) {
        /* allocate first row (row 0) and initialise */
        S1 = (seq_t (*)[7])malloc(sizeof(seq_t[1][7]));
        if (!S1) { abort(); }
        for (int m = 0; m < 7; ++m) S1[0][m] = 0;
        S1[0][0] = (seq_t)1;          /* S1(0,0) = 1 */
        max_row = 0;
        initialized = 1;
    }

    if (max_row >= up_to) return;

    size_t new_rows = (size_t)up_to + 1;
    seq_t *tmp = (seq_t *)realloc(S1, new_rows * 7 * sizeof(seq_t));
    if (!tmp) { abort(); }
    S1 = (seq_t (*)[7])tmp;

    /* initialise new rows to zero */
    for (idx_t i = max_row + 1; i <= up_to; ++i)
        for (int m = 0; m < 7; ++m)
            S1[i][m] = 0;

    /* compute rows from max_row+1 up to up_to using the recurrence */
    for (idx_t k = max_row + 1; k <= up_to; ++k) {
        /* S1(k,0) is already 0 for k>0 */
        for (int m = 1; m <= 6; ++m) {
            S1[k][m] = S1[k-1][m-1] + (seq_t)(k-1) * S1[k-1][m];
        }
    }

    max_row = up_to;
}

seq_t a(idx_t n)
{
    /* need rows up to n+6 to use the formula */
    ensure_up_to(n + 6);
    return S1[n+5][5] + S1[n+5][6];
}
