/*
 * A154843  Triangular array, T(n,k) = s(n,k) + s(n,n-k), where s(n,k) are the
 *          Stirling numbers of the first kind.
 * https://oeis.org/A154843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 21, i16 n <= 45, i32 n <= 105, i64 n <= 233,
 *            i128 n <= 630, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle: T(n,k) = s(n,k) + s(n,n-k), where s(n,k) are Stirling numbers of the first kind.
   a(n) is the linearisation of the triangle row by row.  This implementation
   memoises the rows of the triangle and stores all T(n,k) values in a linear
   cache so that any n can be answered in O(1) after the cache has been built
   up to that point. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state ----------------------------------------------------- */
    static seq_t *cache      = NULL;   /* linear array for all T(n,k) values */
    static size_t cache_len = 0;       /* number of elements in cache    */

    static seq_t *prevS      = NULL;   /* s(n-1,k) for the previous row   */
    static size_t prev_len   = 0;       /* number of elements in prevS     */

    static seq_t *currS      = NULL;   /* temporary buffer for current row*/
    static size_t curr_len   = 0;       /* number of elements in currS     */

    static idx_t  cur_row    = 0;       /* last fully processed row (n)   */

    /* initialise on first call --------------------------------------- */
    if (cache_len == 0) {
        /* row 0 : s(0,0)=1   =>  T(0,0)=1+1=2 */
        cache      = (seq_t *)malloc(sizeof(seq_t));
        cache[0]   = (seq_t)2;
        cache_len  = 1;
        /* prevS holds s(0,0) */
        prevS      = (seq_t *)malloc(sizeof(seq_t));
        prevS[0]   = (seq_t)1;
        prev_len   = 1;
        /* currS will be allocated when row 1 is generated */
    }

    /* if the index is already cached --------------------------------- */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* extend cache until it contains the requested index             */
    while (cache_len <= (size_t)n) {
        /* ---- move to the next row --------------------------------- */
        ++cur_row;                          /* new row number               */
        size_t new_len = (size_t)cur_row + 1;/* number of s(n,k) entries      */

        /* ensure currS is large enough                                */
        if (curr_len < new_len) {
            seq_t *tmp = (seq_t *)realloc(currS, new_len * sizeof(seq_t));
            currS = tmp;
            curr_len = new_len;
        }

        /* compute s(cur_row,k) ------------------------------------- */
        currS[0] = (cur_row > 0) ? (seq_t)0 : (seq_t)1; /* s(n,0) */
        for (size_t k = 1; k < new_len; ++k) {
            seq_t left  = (k-1 < prev_len) ? prevS[k-1] : (seq_t)0;
            seq_t right = (k   < prev_len) ? prevS[k]   : (seq_t)0;
            currS[k] = left - (seq_t)(cur_row - 1) * right;
        }

        /* compute the offset of this row in linear cache              */
        size_t offset = (size_t)cur_row * (cur_row + 1) / 2;

        /* ensure cache is large enough for the whole row              */
        size_t need_len = offset + new_len;
        if (cache_len < need_len) {
            seq_t *tmp = (seq_t *)realloc(cache, need_len * sizeof(seq_t));
            cache = tmp;
            cache_len = need_len;
        }

        /* store T(cur_row,k) = s(cur_row,k) + s(cur_row,cur_row-k)   */
        for (size_t k = 0; k < new_len; ++k) {
            cache[offset + k] = currS[k] + currS[new_len - 1 - k];
        }

        /* swap buffers for next iteration --------------------------- */
        seq_t *swap = prevS;
        prevS = currS;
        currS = swap;
        size_t tmp_len = prev_len;
        prev_len = curr_len;
        curr_len = tmp_len;
    }

    /* now the cache contains the requested entry --------------------- */
    return cache[(size_t)n];
}
