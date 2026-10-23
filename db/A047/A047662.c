/*
 * A047662  Square array a(n,k) read by antidiagonals: a(n,1)=n, a(1,k)=k,
 *          a(n,k)=a(n-1,k-1)+a(n-1,k)+a(n,k-1)+1.
 * https://oeis.org/A047662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 110, u32 n <= 389, u64 n <= 1402,
 *            u128 n <= 5050, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1,1)=1, a(n,1)=n, a(1,k)=k,
   a(n,k)=a(n-1,k-1)+a(n-1,k)+a(n,k-1)+1
   This function returns the n‑th term of the square array read
   by antidiagonals (offset 1). */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t **A = NULL;        /* cached table of a(n,k) values */
static idx_t A_size = 0;        /* current table dimension (rows/cols) */

/* Build or enlarge the table so that indices up to 'needed' are valid. */
static void build_table(idx_t needed)
{
    if (A_size > needed) return;                       /* already large enough */

    idx_t new_size = needed + 1;
    seq_t **newA = (seq_t**)calloc(new_size, sizeof(seq_t*));
    if (!newA) return;                                 /* allocation failure */

    for (idx_t i = 1; i < new_size; ++i) {
        newA[i] = alloc_seq_array(new_size);
        if (!newA[i]) {                                 /* allocation failure */
            /* free already allocated rows */
            for (idx_t k = 1; k < i; ++k) free_seq_array(newA[k]);
            free(newA);
            return;
        }
    }

    /* Compute all entries of the square array */
    for (idx_t i = 1; i < new_size; ++i) {
        for (idx_t j = 1; j < new_size; ++j) {
            if (i == 1) {
                newA[i][j] = (seq_t)j;                 /* a(1,k)=k */
            } else if (j == 1) {
                newA[i][j] = (seq_t)i;                 /* a(n,1)=n */
            } else {
                newA[i][j] = newA[i-1][j-1] + newA[i-1][j]
                            + newA[i][j-1] + (seq_t)1;  /* recurrence */
            }
        }
    }

    /* Free the old table */
    if (A) {
        for (idx_t i = 1; i < A_size; ++i) free_seq_array(A[i]);
        free(A);
    }

    A = newA;
    A_size = new_size;
}

/* Return the n-th element of the array read by antidiagonals. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                                 /* undefined */

    /* Find the antidiagonal s such that the first s*(s-1)/2 terms
       contain the n-th term. */
    idx_t s = 2;
    for (;;) {
        unsigned __int128 prod = (unsigned __int128)s * (s - 1) / 2;
        if (prod >= n) break;
        ++s;
    }

    unsigned __int128 prev =
        (unsigned __int128)(s - 2) * (s - 1) / 2;
    idx_t prev_total = (idx_t)prev;
    idx_t offset = n - prev_total;                       /* 1‑based offset in diagonal */
    idx_t i = offset;
    idx_t j = s - offset;

    idx_t needed = (i > j) ? i : j;
    build_table(needed);

    return A[i][j];
}
