/*
 * A015121  Triangle of q-binomial coefficients for q=-9.
 * https://oeis.org/A015121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 29, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A015121: Triangle of q‑binomial coefficients for q = -9,
   flattened row‑major (triangular array). 
   This implementation builds the triangle iteratively, storing all
   previously computed terms for constant‑time random access.
*/

#include <stdlib.h>

/*  Function prototypes for building the cache and returning values   */
static seq_t *cache = NULL;          /* flattened sequence values       */
static size_t cache_cap = 0;         /* allocated capacity of cache     */
static size_t cache_len = 0;         /* number of values currently cached */

static seq_t *prevRow = NULL;        /* array holding the previous row  */
static size_t prevSize = 0;          /* number of elements in prevRow   */
static seq_t pow_q_row = (seq_t)1;   /* q^r, where r is the current row index   */

/*  Compute a(n) – returns the nth term of the sequence                */
seq_t a(idx_t n)
{
    /*  If the value is already cached, return it immediately.      */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /*  First‑time initialization:  row 0 consists of a single 1.    */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_len = 1;

        prevSize = 1;
        prevRow = (seq_t *)malloc(prevSize * sizeof(seq_t));
        prevRow[0] = (seq_t)1;
        pow_q_row = (seq_t)1;     /* q^0 = 1  */
    }

    /*  Generate new rows until the requested index is reached.     */
    while (cache_len <= (size_t)n) {
        /*  New row will have size prevSize + 1                      */
        size_t newSize = prevSize + 1;
        seq_t *newRow = (seq_t *)malloc(newSize * sizeof(seq_t));

        /*  Update the global power:  pow_q_row = q^(prevSize+1).  */
        pow_q_row = pow_q_row * (seq_t)-9;

        /*  Set the first coefficient (k = 0).                       */
        newRow[0] = (seq_t)1;

        /*  Recurrence:  [n choose k]_q = [n-1 choose k]_q
           + q^(n-k) * [n-1 choose k-1]_q.  For k = 1 .. newSize-2. */
        seq_t pow = pow_q_row;  /* starts as q^n */
        for (size_t k = 1; k < newSize - 1; ++k) {
            pow = pow / (seq_t)-9;   /* now pow = q^(n-k)          */
            newRow[k] = prevRow[k] + pow * prevRow[k - 1];
        }

        /*  Last coefficient (k = n) is always 1.                    */
        newRow[newSize - 1] = (seq_t)1;

        /*  Append the new coefficients to the cache.                 */
        if (cache_len + newSize > cache_cap) {
            size_t newCap = cache_cap * 2 + newSize;
            if (newCap == 0) newCap = newSize;
            cache = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
            cache_cap = newCap;
        }
        for (size_t k = 0; k < newSize; ++k)
            cache[cache_len++] = newRow[k];

        /*  Make this row the previous row for the next iteration.  */
        free(prevRow);
        prevRow = newRow;
        prevSize = newSize;
    }

    /*  All requested values are now cached.                            */
    return cache[(size_t)n];
}
