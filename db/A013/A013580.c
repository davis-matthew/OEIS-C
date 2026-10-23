/*
 * A013580  Triangle formed in same way as Pascal's triangle (A007318) except
 *          1 is added to central element in even-numbered rows.
 * https://oeis.org/A013580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) = value in the modified Pascal triangle * (1 added to the
 *            central element of every even row), * flattened row by row
 *            starting at a(0) = 1
 * Range:     u8 n <= 58, u16 n <= 179, u32 n <= 644, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Ensure idx_t and seq_t are defined by the build. */
seq_t a(idx_t n)
{
    /* Find the row r that contains the index n.
       Row r has r+1 elements; cumulative count up to row r is
       t = r*(r+1)/2.  Locate the smallest r such that t > n,
       then the desired row is r-1. */
    idx_t r = 0;
    while (1) {
        unsigned __int128 t = (unsigned __int128)r * (r + 1) / 2;
        if (t > n) break;
        r++;
    }
    r--;                                   /* actual row index    */
    idx_t start = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - start;                    /* column within row  */

    /* Allocate temporary rows.  Size r+1 is enough for all rows up to r. */
    size_t sz = (size_t)r + 1;
    seq_t *prev = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!prev || !curr) {
        /* On allocation failure, return 0 (should not happen). */
        free(prev); free(curr);
        return (seq_t)0;
    }

    /* Base case: row 0 */
    prev[0] = 1;

    for (idx_t i = 1; i <= r; ++i) {
        curr[0] = 1;
        curr[i] = 1;
        for (idx_t j = 1; j < i; ++j) {
            curr[j] = prev[j - 1] + prev[j];
        }
        /* Increment the central element in an even‑indexed row. */
        if ((i & 1ULL) == 0) {
            idx_t mid = i >> 1;          /* i/2 */
            curr[mid] = curr[mid] + (seq_t)1;
        }
        /* Prepare for next iteration. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[k];

    free(prev);
    free(curr);

    return result;
}
