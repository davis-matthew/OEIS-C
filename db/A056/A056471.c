/*
 * A056471  Number of palindromic structures using a maximum of six different
 *          symbols.
 * https://oeis.org/A056471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..6} A008277(floor((n+1)/2), k) (Stirling numbers
 *            of the second kind)
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 32, u64 n <= 56, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *stirling_table = NULL;   /* 2D table flattened: row i occupies indices i*7 .. i*7+6 */
static idx_t stirling_rows   = 0;      /* number of rows currently stored (next free index) */
static idx_t stirling_cap    = 0;      /* capacity in number of rows */

/* Ensure that Stirling numbers for all n <= m are computed and stored. */
static void ensure_stirling(idx_t m)
{
    if (m < stirling_rows) return;          /* already computed */

    /* Grow capacity to at least m+1 rows, doubling each time */
    if (stirling_cap == 0) stirling_cap = 1;
    while (stirling_cap <= m) stirling_cap <<= 1;

    /* Resize the flat table */
    if (stirling_table == NULL)
        stirling_table = (seq_t*)malloc(stirling_cap * 7 * sizeof(seq_t));
    else
        stirling_table = (seq_t*)realloc(stirling_table, stirling_cap * 7 * sizeof(seq_t));

    /* Zero-initialize any newly allocated rows */
    for (idx_t i = stirling_rows; i < stirling_cap; ++i)
        for (int k = 0; k < 7; ++k)
            stirling_table[i * 7 + k] = 0;

    /* Initialize row 0 if not already present */
    if (stirling_rows == 0) {
        stirling_table[0] = 1;            /* S(0,0) = 1 */
        for (int k = 1; k <= 6; ++k)
            stirling_table[k] = 0;          /* S(0,k>0) = 0 */
        stirling_rows = 1;
    }

    /* Compute rows from the current count up to m */
    for (idx_t n = stirling_rows; n <= m; ++n) {
        seq_t *prev  = &stirling_table[(n - 1) * 7];
        seq_t *curr  = &stirling_table[n * 7];
        curr[0] = 0;                         /* S(n,0) = 0 for n>0 */
        for (int k = 1; k <= 6; ++k) {
            curr[k] = ((seq_t)k) * prev[k] + prev[k - 1];
        }
        stirling_rows = n + 1;
    }
}

seq_t a(idx_t n)
{
    /* m = floor((n+1)/2) */
    idx_t m = (n + 1) >> 1;   /* works because n is integer */

    ensure_stirling(m);

    seq_t sum = 0;
    seq_t *row = &stirling_table[m * 7];
    for (int k = 1; k <= 6; ++k)
        sum += row[k];

    return sum;
}
