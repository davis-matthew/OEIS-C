/*
 * A124961  Triangle read by rows: T(n,k) = mobius(k)*T(n-1,k) + T(n-1,k-1).
 * https://oeis.org/A124961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 177, i16 n <= 721, i32 n <= 3111, i64 n <= 5050,
 *            i128 n <= 5050, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: T(n,k) = μ(k)*T(n‑1,k) + T(n‑1,k‑1).
   The sequence a(n) is the list of the triangle read left‑to‑right, top‑to‑bottom.
   a(1) is the first entry (T(1,1)). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* n < 1 is not required to work; return 0 for safety. */
    if (n == 0) return 0;

    static seq_t *prevRow   = NULL;   /* previous row, length = previous row number */
    static seq_t *currRow   = NULL;   /* current row being constructed */
    static seq_t *flattened = NULL;   /* sequence read by rows */
    static size_t   prevSize     = 0; /* length of previous row (== row number - 1) */
    static size_t   flattenedSize = 0;/* number of values stored */
    static size_t   flattenedCap  = 0;/* capacity of flattened array */
    static idx_t    maxRowComputed = 0;/* rows that have been generated */

    size_t target = (size_t)n;        /* target element index (1‑based) */

    /* Build rows until we have produced at least `target` elements. */
    while (flattenedSize < target) {
        size_t r = ++maxRowComputed;      /* current row number (1‑based) */
        size_t rSize = r;                 /* number of elements in row r */

        /* Allocate / grow the buffer for the current row. */
        seq_t *tmpCurr = (seq_t *)realloc(currRow, rSize * sizeof(seq_t));
        if (!tmpCurr) return 0;           /* allocation failure */
        currRow = tmpCurr;

        if (r == 1) {                     /* base row */
            currRow[0] = 1;
        } else {
            /* Compute the entries using the recurrence. */
            for (size_t j = 0; j < rSize; ++j) {
                size_t k = j + 1;           /* column index (1‑based) */
                seq_t term1 = 0;
                if (j < rSize - 1)          /* T(r‑1,k) exists */
                    term1 = mobius(k) * prevRow[j];
                seq_t term2 = (j > 0) ? prevRow[j - 1] : 0;
                currRow[j] = term1 + term2;
            }
        }

        /* Append the new row to the flattened sequence. */
        if (flattenedSize + rSize > flattenedCap) {
            size_t newCap = flattenedCap ? flattenedCap * 2 : rSize * 2;
            while (newCap < flattenedSize + rSize) newCap *= 2;
            seq_t *tmpFlat = (seq_t *)realloc(flattened, newCap * sizeof(seq_t));
            if (!tmpFlat) return 0;          /* allocation failure */
            flattened = tmpFlat;
            flattenedCap = newCap;
        }
        for (size_t j = 0; j < rSize; ++j) {
            flattened[flattenedSize++] = currRow[j];
        }

        /* Prepare for the next iteration: the current row becomes the previous row. */
        seq_t *tmp = prevRow;
        prevRow = currRow;
        currRow = tmp;
        prevSize = rSize;
    }

    return flattened[target - 1];
}
