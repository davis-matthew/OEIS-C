/*
 * A075504  Stirling2 triangle with scaled diagonals (powers of 9).
 * https://oeis.org/A075504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 46, u64 n <= 123, u128 n <= 382,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A075504: a(n) = 9^(r‑m) * S2(r,m) where r is the row such that
   n lies in the flattened lower‑triangular triangle, and m = n – r(r‑1)/2.
   This implementation builds the required row using the recurrence
   a(r,m) = 9*m*a(r‑1,m) + a(r‑1,m‑1).  It allocates only two rows
   (prev and curr) and discards them after computing a(n).  The
   routine works for any seq_t that supports the arithmetic operators
   and can be allocated with malloc()/free() (including arbitrary‑precision
   integer classes compiled as C++17).
*/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Find the smallest row r such that r*(r+1)/2 >= n */
static idx_t row_of(idx_t n)
{
    idx_t low = 1, high = 1;
    /* Expand high until it covers n */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;
    /* Binary search */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Return the n‑th term of the flattened triangle */
seq_t a(idx_t n)
{
    /* Determine row r and position m */
    idx_t r = row_of(n);
    idx_t prev_sum = (idx_t)((unsigned __int128)(r - 1) * r / 2);
    idx_t pos = n - prev_sum - 1;          /* 0‑based index inside row r */
    idx_t m   = pos + 1;                   /* column index (1‑based) */

    /* Special case: the very first entry */
    if (r == 1)
        return (seq_t)1;

    /* Allocate two rows of size r+1 (index 0 unused) */
    seq_t *prev = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
    if (!prev || !curr) { /* allocation failure – return 0 */
        if (prev) free(prev);
        if (curr) free(curr);
        return (seq_t)0;
    }

    /* Row 1 */
    prev[1] = (seq_t)1;

    /* Build rows 2 .. r */
    for (idx_t cr = 2; cr <= r; ++cr) {
        curr[0] = (seq_t)0;
        for (idx_t cm = 1; cm <= cr; ++cm) {
            seq_t prod = (seq_t)9 * (seq_t)cm;
            seq_t term = prod * ((cm <= cr - 1) ? prev[cm] : (seq_t)0);
            term += prev[cm - 1];
            curr[cm] = term;
        }
        /* Swap prev and curr for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Retrieve the desired value and clean up */
    seq_t result = prev[m];
    free(prev);
    free(curr);
    return result;
}
