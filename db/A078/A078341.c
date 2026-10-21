/*
 * A078341  Triangle read by rows: T(n,k) = n*T(n-1,k-1) + k*T(n-1,k) starting
 *          with T(0,0)=1.
 * https://oeis.org/A078341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 40, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows; recurrence
   T(0,0) = 1
   T(n,k) = n*T(n-1,k-1) + k*T(n-1,k)   for n>=1, 0<=k<=n
   a(n) is the nth element listed row‑wise, offset 1
*/
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

static seq_t **T = NULL;     /* array of pointers to rows */
static idx_t maxRow = (idx_t)-1; /* highest row computed */

/* Compute the nth term of the sequence; n>=1 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Find the row r that contains the n-th element.
       Row r has r+1 elements.  cumulative holds the total
       number of elements up to the current row. */
    idx_t r = 0;
    idx_t cumulative = 1;         /* elements up to row 0 */
    while (cumulative < n) {
        r++;
        cumulative += (r + 1);
    }

    /* Create rows up to r if needed */
    if (T == NULL) {
        T = (seq_t **)malloc(sizeof(seq_t *));
        T[0] = alloc_seq_array(1);
        T[0][0] = (seq_t)1;
        maxRow = 0;
    }
    while (r > maxRow) {
        idx_t newRow = maxRow + 1;
        T = (seq_t **)realloc(T, sizeof(seq_t *) * (newRow + 1));
        T[newRow] = alloc_seq_array(newRow + 1);
        T[newRow][0] = (seq_t)0;            /* T(newRow,0) = 0 for newRow >= 1 */
        for (idx_t k = 1; k <= newRow; ++k) {
            seq_t part1 = (seq_t)newRow * T[newRow - 1][k - 1];
            seq_t part2 = 0;
            if (k <= newRow - 1) {
                part2 = (seq_t)k * T[newRow - 1][k];
            }
            T[newRow][k] = part1 + part2;
        }
        maxRow = newRow;
    }

    /* Index of the element within its row (zero‑based). */
    idx_t prevTotal = (r * (r + 1)) / 2;   /* elements before row r */
    idx_t pos = n - prevTotal - 1;         /* position in row r */
    return T[r][pos];
}
