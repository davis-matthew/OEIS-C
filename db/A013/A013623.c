/*
 * A013623  Triangle of coefficients in expansion of (2 + 7*x)^n.
 * https://oeis.org/A013623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(0)=1, a(n)=flattened coefficients of (2+7x)^n. The
 *            coefficients T(r,k) satisfy: T(0,0)=1;
 *            T(r,k)=7*T(r-1,k-1)+2*T(r-1,k) for 1≤k≤r-1, T(r,0)=2*T(r-1,0),
 *            T(r,r)=7*T(r-1,r-1)
 * Range:     u8 n <= 7, u16 n <= 24, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sparse implementation using only O(r) memory (r≈√(2n)). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r that contains the index n. */
    idx_t start = 0;
    idx_t r = 0;
    /* While the next row would still be within the index range. */
    while (n >= start + r + 1) {
        start += r + 1;   /* new row starts after current row */
        r++;
    }
    /* k is the column within that row */
    idx_t k = n - start;

    /* Allocate two buffers of size r+1.  For r=0 we still allocate a single element. */
    size_t m = (size_t)(r + 1);
    seq_t *buf1 = (seq_t*)malloc(m * sizeof *buf1);
    seq_t *buf2 = (seq_t*)malloc(m * sizeof *buf2);
    if (!buf1 || !buf2) { /* out of memory; return zero as a safe fallback */
        return (seq_t)0;
    }

    /* Use a and b as pointers to the previous and current rows. */
    seq_t *prev = buf1;
    seq_t *curr = buf2;

    /* Initialize the first row T(0,0)=1. */
    prev[0] = (seq_t)1;

    /* Build rows 1..r using the recurrence. */
    for (idx_t row = 1; row <= r; ++row) {
        /* First element of the new row */
        curr[0] = prev[0] * (seq_t)2;
        /* Middle elements */
        for (idx_t j = 1; j < row; ++j) {
            curr[j] = prev[j-1] * (seq_t)7 + prev[j] * (seq_t)2;
        }
        /* Last element of the new row */
        curr[row] = prev[row-1] * (seq_t)7;
        /* Swap buffers for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* prev now points to row r. */
    seq_t result = prev[k];

    /* Free the allocated buffers. */
    free(buf1);
    free(buf2);

    return result;
}
